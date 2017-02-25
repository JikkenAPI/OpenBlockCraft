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

#include <cassert>
#include <GL/glew.h>
#include <jikken/graphicsDevice.hpp>
#include <jikken/jikken.hpp>
#include "platform/glfw/GLFWWindow.hpp"

extern Jikken::GraphicsDevice *gGraphics;

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
	mVsync = false;
	Jikken::API jikkenApi = Jikken::API::eNull;
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
		glfwSwapInterval(mVsync);

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
		jikkenApi = Jikken::API::eOpenGL;		
	}
	else if (graphicsApi == API::eVULKAN)
	{
		jikkenApi = Jikken::API::eVulkan;
	}

	gGraphics = Jikken::createGraphicsDevice(jikkenApi,mWindow);
}

GLFWWindow::~GLFWWindow() 
{
	Jikken::destroyGraphicsDevice(gGraphics);
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

void GLFWWindow::toggleCursor()
{
	mCursorShowing = !mCursorShowing;
	glfwSetInputMode(mWindow, GLFW_CURSOR, mCursorShowing ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void GLFWWindow::toggleVerticalSync()
{
	mVsync = !mVsync;
	glfwSwapInterval(mVsync);
}