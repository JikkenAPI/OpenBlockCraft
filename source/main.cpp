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
#include <glm/gtc/matrix_transform.hpp>
#include "platform/platform.hpp"
#include "platform/windowManager.hpp"
#include "platform/timer.hpp"
#include "platform/input/inputManager.hpp"
#include "core/geometry/cube.hpp"
#include "scene/camera.hpp"

const char *vShader =
"#version 330 core\n"
"\n"
"layout(location = 0) in vec4 vertex;\n"
"\n"
"uniform mat4 mvp;\n"
"\n"
"void main()\n"
"{\n"
"   gl_Position = mvp * vec4(vertex.xyz, 1.0);\n"
"}";

const char *fShader =
"#version 330 core\n"
"\n"
"out vec4 outFragColor;\n"
"\n"
"void main()\n"
"{\n"
"   outFragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
"}\n"
"";

GLuint vbo; // Vertex Buffer Object
GLuint vao; // Vertex Array Object
GLuint ibo; // Index buffer object

GLuint program; // Shader Program

GLuint mvpLoc; // mvp uniform

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
	// For now, just use global state VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_DEPTH_TEST);
	checkGLErrors();
	glDepthFunc(GL_LESS);
	checkGLErrors();

	// create shaders
	{
		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		checkGLErrors();
		glShaderSource(v, 1, &vShader, 0);
		checkGLErrors();
		glCompileShader(v);
		checkGLErrors();
		checkShaderErrors(v, GL_COMPILE_STATUS);
		glShaderSource(f, 1, &fShader, 0);
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
		checkGLErrors();
	}

	// create VBO/VIO of a cube :D
	{
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// fill vbo/vio
		glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVert) * 4 * 6, &sCubeFaceVertices, GL_STATIC_DRAW);

		unsigned short indices[36];
		memset(indices, 0, sizeof(unsigned short) * 36);
		unsigned short index = 0;
		int k = 0;
		// 6 faces / 6 quads
		for (int i = 0; i < 6; i++)
		{
			indices[k++] = index + sCubeFaceIndices[0];
			indices[k++] = index + sCubeFaceIndices[1];
			indices[k++] = index + sCubeFaceIndices[2];
			indices[k++] = index + sCubeFaceIndices[3];
			indices[k++] = index + sCubeFaceIndices[4];
			indices[k++] = index + sCubeFaceIndices[5];

			index += 4;
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * 36, indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		// bind buffers
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBindVertexArray(0);
	}

	mvpLoc = glGetUniformLocation(program, "mvp");

	// set up for rendering.
	glUseProgram(program);
}

void render(Camera *camera, double dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind vao and all of that jazz
	glBindVertexArray(vao);
	checkGLErrors();

	// bind mvp
	glm::mat4 proj = glm::perspective(90.0f, 1440.0f / 900.0f, 0.1f, 200.0f);
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = proj * view * model;
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);
	checkGLErrors();

	// drawcall
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(0));
	checkGLErrors();
}

int main(int argc, const char **argv) 
{
	Platform::initSubSystems();

	Window* window = Platform::getWindowManager()->createWindow(1440, 900);
	Timer *timer = Platform::createTimer();
	InputManager::get()->setTimer(timer);

	// create camera
	Camera *camera = new Camera();
	camera->setPosition(glm::vec3(0.0f, 3.0f, 3.0f));
	InputManager::get()->subscribe(camera, InputEventType::eKEYPRESSEVENT);
	InputManager::get()->subscribe(camera, InputEventType::eMOUSEMOVEMENTEVENT);

	initGL();

	while (!window->shouldClose())
	{
		timer->start();
		Platform::getWindowManager()->processEvents();

		// show delta from last frame (value of timer)
		double fps = timer->getFPS();
		window->setTitle(std::to_string(static_cast<unsigned long>(fps)) + std::string(" fps"));
		
		render(camera, timer->getDelta());

		window->swapBuffers();
		timer->stop();
	}

	Platform::getWindowManager()->destroyWindow(window);
	Platform::destroyTimer(timer);
	Platform::cleanupSubSystems();
	return 0;
}