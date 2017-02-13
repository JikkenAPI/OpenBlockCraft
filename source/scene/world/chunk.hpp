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

#ifndef _SCENE_WORLD_CHUNK_HPP_
#define _SCENE_WORLD_CHUNK_HPP_

#include <vector>
#include <GL/glew.h>
#include <jikken/jikken.hpp>
#include "scene/sceneObject.hpp"
#include "scene/world/block.hpp"
#include "core/geometry/cube.hpp"

const int CHUNK_LENGTH = 16;
const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 256;

class Chunk : public SceneObject
{
public:
	Chunk();
	virtual ~Chunk();

	void genTerrain();
	void genVisibleGeometry();
	void updateTerrainGL();

	void render(Jikken::CommandQueue *cmdQueue, RenderPass pass, const double &dt);

protected:
	// This will allocate a page of memory that is 16x256x16 Block
	Block *mBlocks;

	std::vector<int> mHeightMap;

	bool _isTranslucent(int x, int y, int z);

	void _addFace(int pass, Block &block, const glm::vec3 &pos, const CubeSides &cubeSide);

	struct GL
	{
		Jikken::VertexArrayHandle mVAO;
		Jikken::BufferHandle mVBO;
		Jikken::BufferHandle mIBO;

		std::vector<CubeVert> mVisibleMesh;
		std::vector<uint16_t> mIndexData;

		uint16_t mCurrentIndex;
	};
	GL mGL[2];

	Jikken::LayoutHandle mLayout;
	Jikken::CommandQueue *mUpdateTerrainComamandQueue;
};

#endif