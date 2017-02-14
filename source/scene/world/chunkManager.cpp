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

#include <glm/gtc/matrix_transform.hpp>
#include "scene/world/chunkManager.hpp"

extern Jikken::GraphicsDevice *gGraphics;

// For INITIAL WORLD threaded chunk loading:
// 1. Create first chunk.
// THREADING BEGIN.
// 2. Then, create a bunch of chunks surrounding chunk 0,0,0 chunk->genTerrain
// 3. Wait for terrain to finish genning.
// 4. send all chunks through chunk->getVisibleGeometry() so inner chunk culling
// 5. Execute a callback when visible geometry chunks are done to main thread so
//    it can be uploaded to the GL.
// THREADING END.
// 6. Watch it render pretty colors and cubes.

struct
{
	glm::vec4 direction;
	glm::vec4 ambient;
	glm::vec4 diffuse;
} SunUBOData;

ChunkManager::ChunkManager()
{
	SunUBOData.direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
	SunUBOData.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f);
	SunUBOData.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Create shader.
	std::vector<Jikken::ShaderDetails> details;
	details.push_back({ "../Assets/chunkV.glsl", Jikken::ShaderStage::eVertex });
	details.push_back({ "../Assets/chunkF.glsl", Jikken::ShaderStage::eFragment });
	mShader = gGraphics->createShader(details);

	// Create constant buffers
	mCameraCBuffer = gGraphics->createBuffer(
		Jikken::BufferType::eConstantBuffer,
		Jikken::BufferUsageHint::eDynamicDraw,
		sizeof(glm::mat4) * 2,
		nullptr
	);
	mNormalCBuffer = gGraphics->createBuffer(
		Jikken::BufferType::eConstantBuffer,
		Jikken::BufferUsageHint::eStaticDraw,
		sizeof(glm::vec4) * 6,
		&sCubeFaceNormals[0][0]
	);
	mModelMatrixCBuffer = gGraphics->createBuffer(
		Jikken::BufferType::eConstantBuffer,
		Jikken::BufferUsageHint::eDynamicDraw,
		sizeof(glm::mat4),
		nullptr
	);
	mSunCBuffer = gGraphics->createBuffer(
		Jikken::BufferType::eConstantBuffer,
		Jikken::BufferUsageHint::eStaticDraw,
		sizeof(SunUBOData),
		reinterpret_cast<float*>(&SunUBOData)
	);

	// Bind UBOs to shader
	gGraphics->bindConstantBuffer(mShader, mCameraCBuffer, "Camera", 0);
	gGraphics->bindConstantBuffer(mShader, mNormalCBuffer, "Normals", 1);
	gGraphics->bindConstantBuffer(mShader, mModelMatrixCBuffer, "ChunkModelMatrix", 2);
	gGraphics->bindConstantBuffer(mShader, mSunCBuffer, "Sun", 3);

	mCommandQueue = gGraphics->createCommandQueue();
}

ChunkManager::~ChunkManager()
{
	gGraphics->deleteCommandQueue(mCommandQueue);

	gGraphics->deleteBuffer(mCameraCBuffer);
	gGraphics->deleteBuffer(mNormalCBuffer);
	gGraphics->deleteBuffer(mModelMatrixCBuffer);
	gGraphics->deleteBuffer(mSunCBuffer);
	gGraphics->deleteShader(mShader);
}

void ChunkManager::createChunkAtPosition(const glm::vec3 &pos)
{
	/*
	Chunk *chunk = new Chunk();
	chunk->setPosition(pos);
	
	// TODO: submit to a thread based system to gen chunk.
	chunk->genTerrain();
	chunk->genVisibleGeometry();

	// Operate this on a callback on main thread system.
	chunk->updateTerrainGL();
	mChunks.push_back(chunk);
	*/
}

void ChunkManager::render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix, RenderPass pass, const double &dt)
{
	if (mChunks.size() == 0)
		return;

	// use shader
	auto shaderCmd = mCommandQueue->alloc<Jikken::SetShaderCommand>();
	shaderCmd->handle = mShader;

	// Set view/proj matrix
	glm::mat4 vp[2];
	vp[0] = projMatrix;
	vp[1] = viewMatrix;
	auto viewprojCBuffer = mCommandQueue->alloc<Jikken::UpdateBufferCommand>();
	viewprojCBuffer->buffer = mCameraCBuffer;
	viewprojCBuffer->offset = 0;
	viewprojCBuffer->dataSize = sizeof(glm::mat4) * 2;
	viewprojCBuffer->data = vp;

	// Update frustrum
	mFrustrum.setVP(viewMatrix, projMatrix);

	for (Chunk *chunk : mChunks)
	{
		glm::vec3 size = glm::vec3(CHUNK_LENGTH, CHUNK_HEIGHT, CHUNK_WIDTH) / 2.0f;
		glm::vec3 midPoint = chunk->getPosition() + size;

		// Perform frustrum culling first. If it is inside the frustrum, go ahead and render it.
		if (mFrustrum.checkCubeFAST(midPoint, size))
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, chunk->getPosition());

			// update model matrix ubo
			auto modelCBuffer = mCommandQueue->alloc<Jikken::UpdateBufferCommand>();
			modelCBuffer->buffer = mModelMatrixCBuffer;
			modelCBuffer->offset = 0;
			modelCBuffer->dataSize = sizeof(glm::mat4);
			modelCBuffer->data = mCommandQueue->memcpy(sizeof(glm::mat4), &model[0][0]);

			chunk->render(mCommandQueue, pass, dt);
		}
	}

	// submit command queue
	gGraphics->submitCommandQueue(mCommandQueue);
}

const std::vector<Chunk*>& ChunkManager::getChunks() const
{
	return mChunks;
}

Chunk* ChunkManager::getChunkAtPos(const glm::vec3 &pos)
{
	Chunk *chunk = nullptr;
	for (Chunk *c : mChunks)
	{
		if (c->getPosition() == pos)
		{
			chunk = c;
			break;
		}
	}
	return chunk;
}