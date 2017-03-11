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

#ifndef _SCENE_WORLD_CHUNKMANAGER_HPP_
#define _SCENE_WORLD_CHUNKMANAGER_HPP_

#include <vector>
#include <jikken/jikken.hpp>
#include "core/frustrum.hpp"
#include "scene/world/chunk.hpp"

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

	void createChunkAtPosition(const glm::vec3 &pos);

	void render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix, RenderPass pass, const double &dt);
	//we done rendering all the passes now, submit the queue
	void submitCommandQueue();

	const std::vector<Chunk*>& getChunks() const;

	Chunk* getChunkAtPos(const glm::vec3 &pos);

	void addChunk(Chunk *chunk) { mChunks.push_back(chunk); }

private:
	std::vector<Chunk*> mChunks;
	Frustrum mFrustrum;
	glm::mat4 mViewProjData[2];

	Jikken::CommandQueue *mCommandQueue;
	Jikken::ShaderHandle mShader;
	Jikken::BufferHandle mCameraCBuffer;
	Jikken::BufferHandle mNormalCBuffer;
	Jikken::BufferHandle mModelMatrixCBuffer;
	Jikken::BufferHandle mSunCBuffer;
	//commands
	Jikken::SetShaderCommand mSetShaderCmd;
	Jikken::UpdateBufferCommand mUpdateViewProjBufferCmd;
	Jikken::UpdateBufferCommand mUpdateModelBufferCmd;
};

#endif