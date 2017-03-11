//-----------------------------------------------------------------------------
// MIT License
// 
// Copyright (c) 2017 Jeff Hutchinson
// Copyright (c) 2017 Tim Barnes
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-----------------------------------------------------------------------------

#include <glm/gtc/matrix_transform.hpp>
#include "scene/world/chunkManager.hpp"


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

	// Create command queue
	mCommandQueue = Jikken::createCommandQueue();

	//grab immediate queue
	Jikken::CommandQueue *immQueue = Jikken::getImmediateExecuteQueue();
	// Create shader.
	std::vector<Jikken::ShaderDetails> details;
	//we assume the current working dir is the bin directory
	details.push_back({ "../Assets/chunkV.glsl", Jikken::ShaderStage::eVertex });
	details.push_back({ "../Assets/chunkF.glsl", Jikken::ShaderStage::eFragment });
	mShader = Jikken::createShader(details);

	// Create constant buffers
	mCameraCBuffer = Jikken::createBuffer(Jikken::BufferType::eConstantBuffer);
	mNormalCBuffer = Jikken::createBuffer(Jikken::BufferType::eConstantBuffer);
	mModelMatrixCBuffer = Jikken::createBuffer(Jikken::BufferType::eConstantBuffer);
	mSunCBuffer = Jikken::createBuffer(Jikken::BufferType::eConstantBuffer);
	
	// Allocate the constant buffers
	Jikken::AllocBufferCommand allocCmd = {};
	allocCmd.buffer = mCameraCBuffer;
	allocCmd.dataSize = sizeof(glm::mat4) * 2;
	allocCmd.data = nullptr;
	allocCmd.hint = Jikken::BufferUsageHint::eDynamic;
	immQueue->addAllocBufferCommand(&allocCmd);

	allocCmd.buffer = mNormalCBuffer;
	allocCmd.dataSize = sizeof(glm::vec4) * 6;
	allocCmd.data = &sCubeFaceNormals[0][0];
	allocCmd.hint = Jikken::BufferUsageHint::eStatic;
	immQueue->addAllocBufferCommand(&allocCmd);

	allocCmd.buffer = mModelMatrixCBuffer;
	allocCmd.dataSize = sizeof(glm::mat4);
	allocCmd.data = nullptr;
	allocCmd.hint = Jikken::BufferUsageHint::eDynamic;
	immQueue->addAllocBufferCommand(&allocCmd);

	allocCmd.buffer = mSunCBuffer;
	allocCmd.dataSize = sizeof(SunUBOData);
	allocCmd.data = reinterpret_cast<void*>(&SunUBOData);
	allocCmd.hint = Jikken::BufferUsageHint::eDynamic;
	immQueue->addAllocBufferCommand(&allocCmd);

	//execute the above commands
	Jikken::executeImmediateQueue();

	// Bind UBOs to shader
	Jikken::bindConstantBuffer(mShader, mCameraCBuffer, "Camera", 0);
	Jikken::bindConstantBuffer(mShader, mNormalCBuffer, "Normals", 1);
	Jikken::bindConstantBuffer(mShader, mModelMatrixCBuffer, "ChunkModelMatrix", 2);
	Jikken::bindConstantBuffer(mShader, mSunCBuffer, "Sun", 3);

	mSetShaderCmd.handle = mShader;

	mUpdateViewProjBufferCmd.buffer = mCameraCBuffer;
	mUpdateViewProjBufferCmd.offset = 0;
	mUpdateViewProjBufferCmd.dataSize = sizeof(glm::mat4) * 2;
	mUpdateViewProjBufferCmd.data = mViewProjData;

	mUpdateModelBufferCmd.buffer = mModelMatrixCBuffer;
	mUpdateModelBufferCmd.offset = 0;
	mUpdateModelBufferCmd.dataSize = sizeof(glm::mat4);

}

ChunkManager::~ChunkManager()
{
	for (auto chunk : mChunks)
	{
		delete chunk;
	}

	Jikken::deleteCommandQueue(mCommandQueue);
	Jikken::deleteBuffer(mCameraCBuffer);
	Jikken::deleteBuffer(mNormalCBuffer);
	Jikken::deleteBuffer(mModelMatrixCBuffer);
	Jikken::deleteBuffer(mSunCBuffer);
	Jikken::deleteShader(mShader);
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
	mCommandQueue->addSetShaderCommand(&mSetShaderCmd);

	// Set view/proj matrix
	mViewProjData[0] = projMatrix;
	mViewProjData[1] = viewMatrix;	
	mCommandQueue->addUpdateBufferCommand(&mUpdateViewProjBufferCmd);

	// Update frustrum
	mFrustrum.setVP(viewMatrix, projMatrix);

	for (Chunk *chunk : mChunks)
	{
		glm::vec3 size = glm::vec3(CHUNK_LENGTH, CHUNK_HEIGHT, CHUNK_WIDTH) / 2.0f;
		glm::vec3 midPoint = chunk->getPosition() + size;

		// Perform frustrum culling first. If it is inside the frustrum, go ahead and render it.
		if (mFrustrum.checkCubeFAST(midPoint, size))
		{
			//does this chunk even need rendering? - maybe do this before frustum culling
			if (chunk->needRender(pass))
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, chunk->getPosition());

				// update model matrix ubo
				mUpdateModelBufferCmd.data = &model[0][0];
				mCommandQueue->addUpdateBufferCommand(&mUpdateModelBufferCmd);

				chunk->render(mCommandQueue, pass, dt);
			}
		}
	}
}

void ChunkManager::submitCommandQueue()
{
	Jikken::submitCommandQueue(mCommandQueue);
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