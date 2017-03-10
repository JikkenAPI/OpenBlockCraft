//-----------------------------------------------------------------------------
// OpenBlockCraft -- A 3D block based game
// Copyright(C) 2017 Jeff Hutchinson
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>
//-----------------------------------------------------------------------------

#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include "core/noise.hpp"
#include "scene/world/chunk.hpp"
#include "scene/world/chunkManager.hpp"

// temp
extern ChunkManager *chunkManager;

const int WATER_HEIGHT = 60;

const int MAX_LAND_HEIGHT_FROM_WATER_SURFACE = 48;

const int LAND_HEIGHT_BELOW_WATER = 10;

inline int getCubeIndex(int x, int y, int z)
{
	return x + (z * CHUNK_LENGTH) + (y * CHUNK_LENGTH * CHUNK_WIDTH);
}

Chunk::Chunk()
{
	// Note: A lot of memory.
	const size_t memChunk = sizeof(Block) * CHUNK_LENGTH * CHUNK_WIDTH * CHUNK_HEIGHT;
	mBlocks = reinterpret_cast<Block*>(malloc(memChunk));
	if (mBlocks == nullptr) 
	{
		std::printf("Failure to alloc chunk. Really bad!");
		std::abort();
	}

	// Initialize all memory to the air type block.
	memset(mBlocks, static_cast<uint8_t>(BlockType::eAIR), memChunk);

	std::vector<Jikken::VertexInputLayout> layouts;
	Jikken::VertexInputLayout position;
	position.attribute = Jikken::VertexAttributeName::ePOSITION;
	position.componentSize = 4;
	position.offset = 0;
	position.stride = 0;
	position.type = Jikken::VertexAttributeType::eFLOAT;
	layouts.push_back(position);
	mLayout = Jikken::createVertexInputLayout(layouts);

	mUpdateTerrainComamandQueue = Jikken::createCommandQueue();

	// we have 2 passes. rip me.
	for (int i = 0; i < 2; i++)
	{
		mRenderData[i].mCurrentIndex = 0;

		mRenderData[i].mVBO = Jikken::createBuffer(
			Jikken::BufferType::eVertexBuffer,
			Jikken::BufferUsageHint::eStatic,
			0,
			nullptr
		);

		mRenderData[i].mIBO = Jikken::createBuffer(
			Jikken::BufferType::eIndexBuffer,
			Jikken::BufferUsageHint::eStatic,
			0,
			nullptr
		);

		mRenderData[i].mVAO = Jikken::createVAO(mLayout, mRenderData[i].mVBO, mRenderData[i].mIBO);

		mRenderData[i].mVboReallocCmd.count = 0;
		mRenderData[i].mVboReallocCmd.data = nullptr;
		mRenderData[i].mVboReallocCmd.buffer = mRenderData[i].mVBO;
		mRenderData[i].mVboReallocCmd.stride = sizeof(CubeVert);
		mRenderData[i].mVboReallocCmd.hint = Jikken::BufferUsageHint::eStatic;

		mRenderData[i].mIboReallocCmd.count = 0;
		mRenderData[i].mIboReallocCmd.data = nullptr;
		mRenderData[i].mIboReallocCmd.buffer = mRenderData[i].mIBO;
		mRenderData[i].mIboReallocCmd.stride = sizeof(uint16_t);
		mRenderData[i].mIboReallocCmd.hint = Jikken::BufferUsageHint::eStatic;
	}

	mDrawCmd.primitive = Jikken::PrimitiveType::eTriangles;
	mDrawCmd.start = 0;
}

Chunk::~Chunk()
{
	if (mBlocks != nullptr)
	{
		free(mBlocks);
		mBlocks = nullptr;
	}

	// free GL objects.
	for (int i = 0; i < 2; i++)
	{
		Jikken::deleteBuffer(mRenderData[i].mVBO);
		Jikken::deleteBuffer(mRenderData[i].mIBO);
		Jikken::deleteVAO(mRenderData[i].mVAO);
	}

	Jikken::deleteCommandQueue(mUpdateTerrainComamandQueue);
}

void Chunk::genTerrain()
{
	// First, start off by making height map.
	for (int z = 0; z < CHUNK_WIDTH; ++z)
	{
		for (int x = 0; x < CHUNK_LENGTH; ++x)
		{
			double xOffset = double((mPosition.x / CHUNK_LENGTH) + double(x) / CHUNK_LENGTH) * 0.25;
			double zOffset = double((mPosition.z / CHUNK_WIDTH) + double(z) / CHUNK_WIDTH) * 0.25;

			// calc height and then scale it.
			double height = Noise::get()->noise2(xOffset, zOffset);
			int intHeight = int((((height + 1.0) / 2.0) * MAX_LAND_HEIGHT_FROM_WATER_SURFACE)) + WATER_HEIGHT - LAND_HEIGHT_BELOW_WATER;
			mHeightMap.push_back(intHeight);
		}
	}

	// Then, set water to every block at water height.
	// it will fill in on next pass.
	for (int z = 0; z < CHUNK_WIDTH; ++z)
	{
		for (int x = 0; x < CHUNK_LENGTH; ++x)
		{
			mBlocks[getCubeIndex(x, WATER_HEIGHT, z)].id = BlockType::eWATER;
		}
	}

	for (int z = 0; z < CHUNK_WIDTH; ++z)
	{
		for (int x = 0; x < CHUNK_LENGTH; ++x)
		{
			// Anything above height is air.
			int height = mHeightMap[x + (z * CHUNK_WIDTH)];
			int cubeIndex = getCubeIndex(x, height, z);

			// height layer is grass. yeah.
			mBlocks[cubeIndex].id = BlockType::eDIRT;

			// everything else is dirt.
			for (int y = height - 1; y >= 0; --y)
			{
				cubeIndex = getCubeIndex(x, y, z);
				mBlocks[cubeIndex].id = BlockType::eCOBBLESTONE;
			}
		}
	}
}

void Chunk::genVisibleGeometry()
{
	// Update visible mesh.
	mRenderData[0].mVisibleMesh.clear();
	mRenderData[0].mIndexData.clear();
	mRenderData[0].mVisibleMesh.reserve(16384);
	mRenderData[0].mIndexData.reserve(16384);
	mRenderData[0].mCurrentIndex = 0;

	const std::vector<Chunk*> &chunks = chunkManager->getChunks();

	for (int z = 0; z < CHUNK_WIDTH; ++z)
	{
		for (int x = 0; x < CHUNK_LENGTH; ++x)
		{
			for (int y = 0; y < CHUNK_HEIGHT; ++y)
			{
				Block &block = mBlocks[getCubeIndex(x, y, z)];
				if (_isTranslucent(x, y, z))
					continue;

				// Check all 6 sides to see if there is a translucent material.

				// UP
				if ((y + 1) == CHUNK_HEIGHT || _isTranslucent(x, y + 1, z))
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eUP);

				// DOWN
				if ((y - 1) == -1 || _isTranslucent(x, y - 1, z))
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eDOWN);

				// LEFT
				if ((x - 1) == -1)
				{
					glm::vec3 searchPos = mPosition - glm::vec3(CHUNK_LENGTH, 0.0f, 0.0f);
					Chunk *chunk = chunkManager->getChunkAtPos(searchPos);
					if (chunk != nullptr)
					{
						if (chunk->_isTranslucent(CHUNK_LENGTH - 1, y, z))
						{
							// The chunk to the left is translucent, we have to add this face!
							_addFace(0, block, glm::vec3(x, y, z), CubeSides::eLEFT);
						}
					}
				}
				else if (_isTranslucent(x - 1, y, z))
				{
					// It is translucent.
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eLEFT);
				}

				// RIGHT
				if ((x + 1) == CHUNK_LENGTH)
				{
					glm::vec3 searchPos = mPosition + glm::vec3(CHUNK_LENGTH, 0.0f, 0.0f);
					Chunk *chunk = chunkManager->getChunkAtPos(searchPos);
					if (chunk != nullptr)
					{
						if (chunk->_isTranslucent(0, y, z))
						{
							// The chunk to the left is translucent, we have to add this face!
							_addFace(0, block, glm::vec3(x, y, z), CubeSides::eRIGHT);
						}
					}
				}
				else if (_isTranslucent(x + 1, y, z))
				{
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eRIGHT);
				}

				// FRONT
				if ((z + 1) == CHUNK_WIDTH)
				{
					glm::vec3 searchPos = mPosition + glm::vec3(0.0f, 0.0f, CHUNK_WIDTH);
					Chunk *chunk = chunkManager->getChunkAtPos(searchPos);
					if (chunk != nullptr)
					{
						if (chunk->_isTranslucent(x, y, 0))
						{
							// The chunk to the left is translucent, we have to add this face!
							_addFace(0, block, glm::vec3(x, y, z), CubeSides::eFRONT);
						}
					}
				}
				else if (_isTranslucent(x, y, z + 1))
				{
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eFRONT);
				}

				// BACK
				if ((z - 1) == -1)
				{
					glm::vec3 searchPos = mPosition - glm::vec3(0.0f, 0.0f, CHUNK_WIDTH);
					Chunk *chunk = chunkManager->getChunkAtPos(searchPos);
					if (chunk != nullptr)
					{
						if (chunk->_isTranslucent(x, y, CHUNK_WIDTH - 1))
						{
							// The chunk to the left is translucent, we have to add this face!
							_addFace(0, block, glm::vec3(x, y, z), CubeSides::eBACK);
						}
					}
				}
				else if (_isTranslucent(x, y, z - 1))
				{
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eBACK);
				}
			}
		}
	}

	// Now, we fill the water buffer with data.
	// for now we just do the top layer of the water, not the sides.
	mRenderData[1].mVisibleMesh.clear();
	mRenderData[1].mIndexData.clear();
	mRenderData[1].mVisibleMesh.reserve(16384);
	mRenderData[1].mIndexData.reserve(16384);
	mRenderData[1].mCurrentIndex = 0;

	for (int z = 0; z < CHUNK_WIDTH; ++z)
	{
		for (int x = 0; x < CHUNK_LENGTH; ++x)
		{
			Block &block = mBlocks[getCubeIndex(x, WATER_HEIGHT, z)];

			// If the block already has something, no water!
			if (block.id != BlockType::eWATER)
				continue;

			// Make it water baby!
			_addFace(1, block, glm::vec3(x, WATER_HEIGHT, z), CubeSides::eUP);
		}
	}
}

bool Chunk::_isTranslucent(int x, int y, int z)
{
	uint8_t type = mBlocks[getCubeIndex(x, y, z)].id;
	if (type == BlockType::eAIR)
		return true;
	if (type == BlockType::eWATER)
		return true;
	return false;
}

void Chunk::_addFace(int pass, Block &block, const glm::vec3 &pos, const CubeSides &cubeSide)
{
	// 4 verts per face
	for (int i = 0; i < 4; ++i)
	{
		CubeVert vert;
		vert.pos = pos + sCubeFaceVertices[cubeSide][i].pos;
		vert.normal = sCubeFaceVertices[cubeSide][i].normal;
		vert.textureID = block.id;
		mRenderData[pass].mVisibleMesh.push_back(vert);
	}

	// 6 indices per face
	for (int i = 0; i < 6; ++i)
		mRenderData[pass].mIndexData.push_back(sCubeFaceIndices[i] + mRenderData[pass].mCurrentIndex);
	mRenderData[pass].mCurrentIndex += 4;
}

void Chunk::updateTerrainGL()
{
	for (int i = 0; i < 2; i++)
	{
		mRenderData[i].mVboReallocCmd.count = mRenderData[i].mVisibleMesh.size();
		mRenderData[i].mVboReallocCmd.data = mRenderData[i].mVisibleMesh.data();
		mUpdateTerrainComamandQueue->addReallocBufferCommand(&mRenderData[i].mVboReallocCmd);

		mRenderData[i].mIboReallocCmd.count = mRenderData[i].mIndexData.size();
		mRenderData[i].mIboReallocCmd.data = mRenderData[i].mIndexData.data();
		mUpdateTerrainComamandQueue->addReallocBufferCommand(&mRenderData[i].mIboReallocCmd);
	}
	Jikken::submitCommandQueue(mUpdateTerrainComamandQueue);
}

void Chunk::render(Jikken::CommandQueue *cmdQueue, RenderPass pass, const double &dt)
{
	int renderPass = (pass == RenderPass::eGEOMETRY) ? 0 : 1;
	if (mRenderData[renderPass].mIndexData.size() == 0)
	{
		//printf("pass %d has 0 data! We can save from rendering this!\n");
		return;
	}

	// Bind VAO
	mBindVaoCmd.vertexArray = mRenderData[renderPass].mVAO;
	cmdQueue->addBindVAOCommand(&mBindVaoCmd);

	// Issue Drawcall
	mDrawCmd.count = static_cast<uint32_t>(mRenderData[renderPass].mIndexData.size());
	cmdQueue->addDrawCommand(&mDrawCmd);
}