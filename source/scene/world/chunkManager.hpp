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