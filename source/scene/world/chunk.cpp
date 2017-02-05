//-----------------------------------------------------------------------------
// chunk.cpp
//
// Copyright(c) 2017 Jeff Hutchinson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-----------------------------------------------------------------------------

#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include "core/noise.hpp"
#include "scene/world/chunk.hpp"

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

	// we have 2 passes. rip me.
	for (int i = 0; i < 2; i++)
	{
		mGL[i].mCurrentIndex = 0;

		glGenBuffers(1, &mGL[i].mVBO);
		glGenBuffers(1, &mGL[i].mIBO);

		glGenVertexArrays(1, &mGL[i].mVAO);
		glBindVertexArray(mGL[i].mVAO);

		// The following state is captured by the bound vertex array.
		// When we rebind the vertex array, this will all be implicitly
		// bound by the driver.
		{
			glBindBuffer(GL_ARRAY_BUFFER, mGL[i].mVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGL[i].mIBO);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}
		glBindVertexArray(0);
	}
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
		glDeleteBuffers(1, &mGL[i].mVBO);
		glDeleteBuffers(1, &mGL[i].mIBO);
		glDeleteVertexArrays(1, &mGL[i].mVAO);
	}
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
	mGL[0].mVisibleMesh.clear();
	mGL[0].mIndexData.clear();
	mGL[0].mCurrentIndex = 0;

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
				if ((x - 1) == -1 || _isTranslucent(x - 1, y, z))
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eLEFT);

				// RIGHT
				if ((x + 1) == CHUNK_LENGTH || _isTranslucent(x + 1, y, z))
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eRIGHT);

				// FRONT
				if ((z + 1) == CHUNK_WIDTH || _isTranslucent(x, y, z + 1))
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eFRONT);

				// BACK
				if ((z - 1) == -1 || _isTranslucent(x, y, z - 1))
					_addFace(0, block, glm::vec3(x, y, z), CubeSides::eBACK);
			}
		}
	}

	// Now, we fill the water buffer with data.
	// for now we just do the top layer of the water, not the sides.
	mGL[1].mVisibleMesh.clear();
	mGL[1].mIndexData.clear();
	mGL[1].mCurrentIndex = 0;

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
		mGL[pass].mVisibleMesh.push_back(vert);
	}

	// 6 indices per face
	for (int i = 0; i < 6; ++i)
		mGL[pass].mIndexData.push_back(sCubeFaceIndices[i] + mGL[pass].mCurrentIndex);
	mGL[pass].mCurrentIndex += 4;
}

void Chunk::updateTerrainGL()
{
	for (int i = 0; i < 2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mGL[i].mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVert) * mGL[i].mVisibleMesh.size(), mGL[i].mVisibleMesh.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGL[i].mIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * mGL[i].mIndexData.size(), mGL[i].mIndexData.data(), GL_STATIC_DRAW);
	}
}

extern GLuint modelLoc; // mvp uniform
extern void checkGLErrors();

void Chunk::render(RenderPass pass, const double &dt)
{
	int renderPass = (pass == RenderPass::eGEOMETRY) ? 0 : 1;

	glBindVertexArray(mGL[renderPass].mVAO);

	glm::mat4 model(1.0f);
	model = glm::translate(model, mPosition);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	glDrawElements(GL_TRIANGLES, mGL[renderPass].mIndexData.size(), GL_UNSIGNED_SHORT, nullptr);
}