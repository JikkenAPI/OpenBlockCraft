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

#ifndef _SCENE_WORLD_CHUNK_HPP_
#define _SCENE_WORLD_CHUNK_HPP_

#include <vector>
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
	void updateTerrain();

	void render(Jikken::CommandQueue *cmdQueue, RenderPass pass, const double &dt);

	bool needRender(RenderPass pass);

protected:
	// This will allocate a page of memory that is 16x256x16 Block
	Block *mBlocks;

	std::vector<int> mHeightMap;

	bool _isTranslucent(int x, int y, int z);

	void _addFace(int pass, Block &block, const glm::vec3 &pos, const CubeSides &cubeSide);

	struct RenderData
	{
		Jikken::VertexArrayHandle mVAO;
		Jikken::BufferHandle mVBO;
		Jikken::BufferHandle mIBO;

		Jikken::AllocBufferCommand mVboAllocCmd;
		Jikken::AllocBufferCommand mIboAllocCmd;

		std::vector<CubeVert> mVisibleMesh;
		std::vector<uint16_t> mIndexData;

		uint16_t mCurrentIndex;
	};
	RenderData mRenderData[2];

	Jikken::LayoutHandle mLayout;
	Jikken::CommandQueue *mUpdateTerrainComamandQueue;

	Jikken::DrawCommand mDrawCmd;
	Jikken::BindVAOCommand mBindVaoCmd;
};

#endif