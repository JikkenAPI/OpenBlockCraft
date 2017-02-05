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
#include "core/noise.hpp"
#include "scene/world/chunk.hpp"

inline int getCubeIndex(int x, int y, int z)
{
	return x + (z * CHUNK_LENGTH) + (y * CHUNK_LENGTH * CHUNK_WIDTH);
}

Chunk::Chunk() :
	mCurrentIndex(0)
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

	glGenBuffers(1, &mGL.mVBO);
	glGenBuffers(1, &mGL.mIBO);

	glGenVertexArrays(1, &mGL.mVAO);
	glBindVertexArray(mGL.mVAO);

	// The following state is captured by the bound vertex array.
	// When we rebind the vertex array, this will all be implicitly
	// bound by the driver.
	{
		glBindBuffer(GL_ARRAY_BUFFER, mGL.mVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGL.mIBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}
	glBindVertexArray(0);
}

Chunk::~Chunk()
{
	if (mBlocks != nullptr)
	{
		free(mBlocks);
		mBlocks = nullptr;
	}

	// free GL objects.
	glDeleteBuffers(1, &mGL.mVBO);
	glDeleteBuffers(1, &mGL.mIBO);
	glDeleteVertexArrays(1, &mGL.mVAO);
}

void Chunk::genHeightMap()
{
	// First, start off by making height map.
	for (int z = 0; z < CHUNK_WIDTH; ++z)
	{
		for (int x = 0; x < CHUNK_LENGTH; ++x)
		{
			double xOffset = double((mPosition.x / CHUNK_LENGTH) + double(x) / CHUNK_LENGTH) * 0.5;
			double zOffset = double((mPosition.z / CHUNK_WIDTH) + double(z) / CHUNK_WIDTH) * 0.5;

			// calc height and then scale it.
			double height = Noise::get()->noise2(xOffset, zOffset);
			int intHeight = int((((height + 1.0) / 2.0) * CHUNK_HEIGHT));
			mHeightMap.push_back(intHeight);
		}
	}

	genTerrain();
}

void Chunk::genTerrain()
{
	for (int z = 0; z < CHUNK_WIDTH; ++z)
	{
		for (int x = 0; x < CHUNK_LENGTH; ++x)
		{
			// Anything above height is air.
			int height = mHeightMap[z + (x * CHUNK_WIDTH)];
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

	// Update visible mesh.
	mVisibleMesh.clear();
	mIndexData.clear();
	mCurrentIndex = 0;

#define isAir(_x, _y, _z) (mBlocks[getCubeIndex(_x, _y, _z)].id == BlockType::eAIR)

	for (int z = 0; z < CHUNK_WIDTH; ++z)
	{
		for (int x = 0; x < CHUNK_LENGTH; ++x)
		{
			for (int y = 0; y < CHUNK_HEIGHT; ++y)
			{
				Block &block = mBlocks[getCubeIndex(x, y, z)];
				if (block.id == BlockType::eAIR)
					continue;

				// Check all 6 sides to see if there is air.

				// UP
				if ((y + 1) == CHUNK_HEIGHT || isAir(x, y + 1, z))
					_addFace(block, glm::vec3(x, y, z), CubeSides::eUP);

				// DOWN
				if ((y - 1) == -1 || isAir(x, y - 1, z))
					_addFace(block, glm::vec3(x, y, z), CubeSides::eDOWN);

				// LEFT
				if ((x - 1) == -1 || isAir(x - 1, y, z))
					_addFace(block, glm::vec3(x, y, z), CubeSides::eLEFT);

				// RIGHT
				if ((x + 1) == CHUNK_LENGTH || isAir(x + 1, y, z))
					_addFace(block, glm::vec3(x, y, z), CubeSides::eRIGHT);

				// FRONT
				if ((z + 1) == CHUNK_WIDTH || isAir(x, y, z + 1))
					_addFace(block, glm::vec3(x, y, z), CubeSides::eFRONT);

				// BACK
				if ((z - 1) == -1 || isAir(x, y, z - 1))
					_addFace(block, glm::vec3(x, y, z), CubeSides::eBACK);
			}
		}
	}

#undef isAir
}

void Chunk::_addFace(Block &block, const glm::vec3 &pos, const CubeSides &cubeSide)
{
	printf("Building %d face: %f %f %f\n", cubeSide, pos.x, pos.y, pos.z);

	// 4 verts per face
	for (int i = 0; i < 4; ++i)
	{
		CubeVert vert;
		vert.pos = pos + sCubeFaceVertices[cubeSide][i].pos;
		vert.normal = sCubeFaceVertices[cubeSide][i].normal;
		vert.textureID = block.id;
		mVisibleMesh.push_back(vert);
	}

	// 6 indices per face
	for (int i = 0; i < 6; ++i)
		mIndexData.push_back(sCubeFaceIndices[i] + mCurrentIndex);
	mCurrentIndex += 4;
}

void Chunk::updateTerrainGL()
{
	glBindBuffer(GL_ARRAY_BUFFER, mGL.mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVert) * mVisibleMesh.size(), mVisibleMesh.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGL.mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * mIndexData.size(), mIndexData.data(), GL_STATIC_DRAW);
}

extern GLuint modelLoc; // mvp uniform
extern void checkGLErrors();

void Chunk::render(const double &dt)
{
	glBindVertexArray(mGL.mVAO);

	glm::mat4 model(1.0f);
	model = glm::translate(model, mPosition);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	glDrawElements(GL_TRIANGLES, mIndexData.size(), GL_UNSIGNED_SHORT, nullptr);
}