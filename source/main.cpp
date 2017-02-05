//-----------------------------------------------------------------------------
// main.cpp
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

#include <cstdlib>
#include <string>
#include <GL/glew.h>
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

GLuint ubo; // UBO for projection and view matrices (Camera)
GLuint normalUBO; // Ubo for normals.
GLuint sunUBO; // ubo for sun

GLuint program; // Shader Program

GLuint modelLoc; // mvp uniform

ChunkManager *chunkManager = nullptr;

const int UBO_CAMERA_LOCATION = 0;
const int UBO_NORMALS_LOCATION = 1;
const int UBO_SUN_LOCATION = 2;

struct
{
	glm::vec4 direction;
	glm::vec4 ambient;
	glm::vec4 diffuse;
} SunUBOData;

bool checkShaderErrors(GLuint shader, GLenum status)
{
	GLint success;
	glGetShaderiv(shader, status, &success);
	if (!success)
	{
		const int LENGTH = 1024;
		GLchar info[LENGTH];
		glGetShaderInfoLog(shader, LENGTH, 0, info);
		printf("Shader Error:\n%s\n", info);
		return false;
	}
	return true;
}

void checkGLErrors()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("GL error: %i\n", err);
	}
}

void initGL()
{
	File vShader("Assets/chunkV.glsl", File::AccessFlags::eREAD);
	File fShader("Assets/chunkF.glsl", File::AccessFlags::eREAD);

	if (!vShader.isFile() || !fShader.isFile())
	{
		std::printf("Unable to find shaders!");
		assert(false);
		return;
	}

	glEnable(GL_DEPTH_TEST);
	checkGLErrors();
	glDepthFunc(GL_LESS);
	checkGLErrors();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// create shaders
	{
		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		checkGLErrors();
		std::string vContents;
		vShader.readFile(vContents);
		const char *contents = reinterpret_cast<const char*>(vContents.c_str());
		glShaderSource(v, 1, &contents, 0);
		checkGLErrors();
		glCompileShader(v);
		checkGLErrors();
		checkShaderErrors(v, GL_COMPILE_STATUS);
		std::string fContents;
		fShader.readFile(fContents);
		const char *fContents2 = reinterpret_cast<const char*>(fContents.c_str());
		glShaderSource(f, 1, &fContents2, 0);
		checkGLErrors();
		glCompileShader(f);
		checkGLErrors();
		checkShaderErrors(f, GL_COMPILE_STATUS);

		program = glCreateProgram();
		checkGLErrors();
		glAttachShader(program, v);
		checkGLErrors();
		glAttachShader(program, f);
		checkGLErrors();
		glLinkProgram(program);

		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLint len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
			char *msg = new char[len + 1];
			memset(msg, 0, sizeof(char) * (len + 1));
			glGetProgramInfoLog(program, len, &len, msg);
			printf("GL Link Issue: %s\n", msg);
			delete[] msg;
		}

		checkGLErrors();
	}

	modelLoc = glGetUniformLocation(program, "model");
	checkGLErrors();

	// ubo
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_DYNAMIC_DRAW);
	checkGLErrors();

	// bind ubo
	glUniformBlockBinding(program, glGetUniformBlockIndex(program, "Camera"), UBO_CAMERA_LOCATION);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_CAMERA_LOCATION, ubo);
	checkGLErrors();

	// normal ubo
	glGenBuffers(1, &normalUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, normalUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 6, &sCubeFaceNormals[0][0], GL_STATIC_DRAW);
	checkGLErrors();

	// bind ubo
	glUniformBlockBinding(program, glGetUniformBlockIndex(program, "Normals"), UBO_NORMALS_LOCATION);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_NORMALS_LOCATION, normalUBO);
	checkGLErrors();

	{
		SunUBOData.direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
		SunUBOData.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f);
		SunUBOData.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		glGenBuffers(1, &sunUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, sunUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(SunUBOData), &SunUBOData, GL_STATIC_DRAW);
		checkGLErrors();

		glUniformBlockBinding(program, glGetUniformBlockIndex(program, "Sun"), UBO_SUN_LOCATION);
		glBindBufferBase(GL_UNIFORM_BUFFER, UBO_SUN_LOCATION, sunUBO);
	}

	// bind proj matrix since it doesn't change yet :)
	glm::mat4 proj = glm::perspective(90.0f, 1440.0f / 900.0f, 0.1f, 500.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &proj[0][0]);
	checkGLErrors();

	// set up for rendering.
	glUseProgram(program);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void render(Camera *camera, double dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkGLErrors();

	// bind view matrix
	glm::mat4 view = camera->getViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0][0]);
	checkGLErrors();

	// Geometry pass first, then translucent.
	chunkManager->render(RenderPass::eGEOMETRY, dt);
	chunkManager->render(RenderPass::eTRANSLUCENT, dt);
}

void createChunks()
{
	// spawn a chunk
	const int grid = 12;
	for (int x = -CHUNK_LENGTH * grid; x < CHUNK_LENGTH * grid; x += CHUNK_LENGTH)
	{
		for (int z = -CHUNK_WIDTH * grid; z < CHUNK_WIDTH * grid; z += CHUNK_WIDTH)
		{
			chunkManager->createChunkAtPosition(glm::vec3(x, 0, z));
		}
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

	// global VAO, at least one needs to be bound
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

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
	glDeleteVertexArrays(1, &vao);
	Platform::getWindowManager()->destroyWindow(window);
	Platform::destroyTimer(timer);
	Platform::cleanupSubSystems();
	return 0;
}