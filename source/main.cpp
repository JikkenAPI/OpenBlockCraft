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

#include <thread>
#include <cstdlib>
#include <string>
#include <jikken/jikken.hpp>
#include <open-simplex-noise.h>
#include <glm/gtc/matrix_transform.hpp>
#include "platform/platform.hpp"
#include "platform/windowManager.hpp"
#include "platform/timer.hpp"
#include "platform/input/inputManager.hpp"
#include "core/geometry/cube.hpp"
#include "scene/camera.hpp"
#include "core/file.hpp"
#include "scene/world/chunkManager.hpp"
#include "core/noise.hpp"

Jikken::CommandQueue *queue = nullptr;
ChunkManager *chunkManager = nullptr;
IWindow *window = nullptr;
Timer *timer = nullptr;
Jikken::BeginFrameCommand beginFrameCmd;

void init()
{
	queue = Jikken::createCommandQueue();
	Jikken::DepthStencilStateCommand depthCmd;
	depthCmd.depthEnabled = true;
	depthCmd.depthWrite = true;
	depthCmd.depthFunc = Jikken::DepthFunc::eLess;

	Jikken::CullStateCommand cullCmd;
	cullCmd.enabled = true;
	cullCmd.face = Jikken::CullFaceState::eBack;
	cullCmd.state = Jikken::WindingOrderState::eCCW;

	Jikken::BlendStateCommand blendCmd;
	blendCmd.enabled = true;
	blendCmd.source = Jikken::BlendState::eSrcAlpha;
	blendCmd.dest = Jikken::BlendState::eOneMinusSrcAlpha;

	//record commands
	Jikken::CommandQueue *immQueue = Jikken::getImmediateExecuteQueue();
	immQueue->addDepthStencilStateCommand(&depthCmd);
	immQueue->addCullStateCommand(&cullCmd);
	immQueue->addBlendStateCommand(&blendCmd);
	Jikken::executeImmediateQueue();


	beginFrameCmd.clearFlag = Jikken::ClearBufferFlags::eColor | Jikken::ClearBufferFlags::eDepth;
	const float clearColor[4] = { 0.329412f, 0.329412f, 0.329412f, 1.0f };
	//set  beginFrameCmd.clearColor
	memcpy(beginFrameCmd.clearColor, clearColor, sizeof(clearColor));
	beginFrameCmd.depth = 1.0f;
	beginFrameCmd.stencil = 0;
}

void render(Camera *camera, double dt)
{
	// begin frame command and clear default framebuffer
	queue->addBeginFrameCommand(&beginFrameCmd);
	Jikken::submitCommandQueue(queue);

	const glm::mat4 &view = camera->getViewMatrix();
	glm::mat4 proj = proj = glm::perspective(90.0f, 1440.0f / 900.0f, 0.1f, 500.0f);

	// Geometry pass first, then translucent.
	//todo - multithread this and have seperate queue for each pass and process eGEOMETRY & eTRANSLUCENT on different threads
	chunkManager->render(view, proj, RenderPass::eGEOMETRY, dt);
	chunkManager->render(view, proj, RenderPass::eTRANSLUCENT, dt);
	//we share the command queue between passes so we submit when all passes are complete
	chunkManager->submitCommandQueue();
}

void createChunks()
{
	std::vector<Chunk*> chunks;

	// spawn chunks.
	const int grid = 8;
	for (int x = -CHUNK_LENGTH * grid; x < CHUNK_LENGTH * grid; x += CHUNK_LENGTH)
	{
		for (int z = -CHUNK_WIDTH * grid; z < CHUNK_WIDTH * grid; z += CHUNK_WIDTH)
		{
			Chunk *chunk = new Chunk();
			chunk->setPosition(glm::vec3(x, 0, z));
			chunk->genTerrain();
			chunks.push_back(chunk);
			chunkManager->addChunk(chunk);
		}
	}

	const int NUM_THREADS = 8;

	// now generate geometry for each chunk.
	std::vector<Chunk*> chunksss[NUM_THREADS];
	int i = 0;
	for (Chunk *c : chunks)
	{
		chunksss[++i % NUM_THREADS].push_back(c);
	}

	std::vector<std::thread*> threads;
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		std::thread *thr = new std::thread([](const std::vector<Chunk*> &ch)
		{
			for (Chunk * cccc : ch)
			{
				cccc->genVisibleGeometry();
			}
		}, chunksss[i]);
		threads.push_back(thr);
	}

	// wait to finish.
	for (std::thread *thr : threads)
	{
		thr->join();
		delete thr;
	}

	// upload to Jikken
	for (Chunk *c : chunks)
	{
		c->updateTerrain();
	}
}

void cleanup()
{
	delete chunkManager;
	Jikken::deleteCommandQueue(queue);
	Jikken::shutdown();
	Platform::getWindowManager()->destroyWindow(window);
	Platform::destroyTimer(timer);
	Platform::cleanupSubSystems();
}

int main(int argc, const char **argv)
{
    Platform::setWorkingDirectory(argv[0]);
	Platform::initSubSystems();

	window = Platform::getWindowManager()->createWindow(1440, 900);
	timer = Platform::createTimer();
	InputManager::get()->setTimer(timer);
	InputManager::get()->subscribe(window, InputEventType::eKEYPRESSEVENT);
	Noise::get()->setSeed(696969);
	//init jikken
	Jikken::NativeWindowData wd = window->getJikkenNativeWindowData();
	Jikken::ContextConfig cc;
#ifdef _DEBUG
	cc.debugEnabled = true;
#endif
	if (!Jikken::init(Jikken::GraphicsApi::eOpenGL, cc, wd))
	{
		std::printf("Failed to init Jikken\n");
		cleanup();
		return 1;
	}

	//init chunk manager
	chunkManager = new ChunkManager();

	// create camera
	Camera *camera = new Camera();
	camera->setPosition(glm::vec3(0.0f, 100.0f, 3.0f));
	InputManager::get()->subscribe(camera, InputEventType::eKEYPRESSEVENT);
	InputManager::get()->subscribe(camera, InputEventType::eMOUSEMOVEMENTEVENT);

	init();

	createChunks();
	
	while (!window->shouldClose())
	{
		timer->start();
		Platform::getWindowManager()->processEvents();

		// show delta from last frame (value of timer)
		double fps = timer->getFPS();
		window->setTitle(std::to_string(static_cast<unsigned long>(fps)) + std::string(" fps"));

		// update camera and other objects
		camera->update(timer->getDelta());

		render(camera, timer->getDelta());

		//execute the lot and present to screen
		Jikken::execute(true);

		timer->stop();
	}

	cleanup();
	return 0;
}
