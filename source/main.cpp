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
#include <jikken/graphicsDevice.hpp>
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

Jikken::GraphicsDevice *gGraphics = nullptr;
ChunkManager *chunkManager = nullptr;

void initGL()
{
	Jikken::CommandQueue *queue = gGraphics->createCommandQueue();
	auto depthCmd = queue->alloc<Jikken::DepthStencilStateCommand>();
	depthCmd->depthEnabled = true;
	depthCmd->depthWrite = true;
	depthCmd->depthFunc = Jikken::DepthFunc::eLess;

	auto cullCmd = queue->alloc<Jikken::CullStateCommand>();
	cullCmd->enabled = true;
	cullCmd->face = Jikken::CullFaceState::eBack;
	cullCmd->state = Jikken::WindingOrderState::eCCW;

	auto blendCmd = queue->alloc<Jikken::BlendStateCommand>();
	blendCmd->enabled = true;
	blendCmd->source = Jikken::BlendState::eSrcAlpha;
	blendCmd->dest = Jikken::BlendState::eOneMinusSrcAlpha;

	gGraphics->submitCommandQueue(queue);
	gGraphics->deleteCommandQueue(queue);
}

void render(Camera *camera, double dt)
{
	const glm::mat4 &view = camera->getViewMatrix();
	glm::mat4 proj = proj = glm::perspective(90.0f, 1440.0f / 900.0f, 0.1f, 500.0f);

	// Geometry pass first, then translucent.
	chunkManager->render(view, proj, RenderPass::eGEOMETRY, dt);
	//chunkManager->render(view, proj, RenderPass::eTRANSLUCENT, dt);
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
		std::thread *thr = new std::thread([](std::vector<Chunk*> &ch)
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

	// upload to GL
	for (Chunk *c : chunks)
	{
		c->updateTerrainGL();
	}
}

int main(int argc, const char **argv) 
{
	Platform::initSubSystems();

	Window* window = Platform::getWindowManager()->createWindow(1440, 900);
	Timer *timer = Platform::createTimer();
	InputManager::get()->setTimer(timer);
	InputManager::get()->subscribe(window, InputEventType::eKEYPRESSEVENT);
	Noise::get()->setSeed(696969);
	chunkManager = new ChunkManager();

	// create camera
	Camera *camera = new Camera();
	camera->setPosition(glm::vec3(0.0f, 100.0f, 3.0f));
	InputManager::get()->subscribe(camera, InputEventType::eKEYPRESSEVENT);
	InputManager::get()->subscribe(camera, InputEventType::eMOUSEMOVEMENTEVENT);

	initGL();

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

		window->swapBuffers();
		timer->stop();
	}

	delete chunkManager;
	Platform::getWindowManager()->destroyWindow(window);
	Platform::destroyTimer(timer);
	Platform::cleanupSubSystems();
	return 0;
}