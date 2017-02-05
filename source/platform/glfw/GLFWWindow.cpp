//-----------------------------------------------------------------------------
// GLFWWindow.cpp
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

#include <cassert>
#include <GL/glew.h>
#include "platform/glfw/GLFWWindow.hpp"

#if defined(_DEBUG) && !defined(__APPLE__)
static void APIENTRY debugGLCb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			printf("[OpenGL] Severe Error: %s\n", message);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			printf("[OpenGL] Medium Error: %s\n", message);
			break;
		case GL_DEBUG_SEVERITY_LOW:
			printf("[OpenGL] Low Error: %s\n", message);
			break;
		//default:
		//	printf("[OpenGL] Uknown: %s\n", message);
	}
}
#endif

GLFWWindow::GLFWWindow(int width, int height, API graphicsApi) 
{
	// set what kind of graphics we will need.
	if (graphicsApi == API::eOPENGL)
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	else
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// set up opengl context information
	if (graphicsApi == API::eOPENGL)
	{
		// We need at least a 3.3 core profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		// Request a debug context. At this time, MacOS does not have support
		// for debug contexts.
#if defined(_DEBUG) && !defined(__APPLE__)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	}

	mWindow = glfwCreateWindow(width, height, "", nullptr, nullptr);
	if (graphicsApi == API::eOPENGL)
	{
		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(0);

		// start up glew
		if (glewInit() != GLEW_OK)
		{
			// Unable to load GLEW
			assert(false);
		}

#if defined(_DEBUG) && !defined(__APPLE__)
		if (glewIsExtensionSupported("GL_KHR_debug"))
		{
			glDebugMessageCallback(debugGLCb, nullptr);
		}
#endif
	}
}

GLFWWindow::~GLFWWindow() 
{
	glfwDestroyWindow(mWindow);
}

void GLFWWindow::setTitle(const std::string &title)
{
	glfwSetWindowTitle(mWindow, title.c_str());
}

bool GLFWWindow::shouldClose() const
{
	return !!glfwWindowShouldClose(mWindow);
}

void GLFWWindow::swapBuffers()
{
	glfwSwapBuffers(mWindow);
}

void GLFWWindow::toggleCursor()
{
	mCursorShowing = !mCursorShowing;
	glfwSetInputMode(mWindow, GLFW_CURSOR, mCursorShowing ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}