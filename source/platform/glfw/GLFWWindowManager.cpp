//-----------------------------------------------------------------------------
// GLFWWindowManager.cpp
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
#include "platform/glfw/GLFWWindowManager.hpp"
#include "platform/glfw/GLFWWindow.hpp"

GLFWWindowManager::GLFWWindowManager()
{

}

GLFWWindowManager::~GLFWWindowManager()
{

}

Window* GLFWWindowManager::createWindow(int width, int height, Window::API graphicsApi)
{
	GLFWWindow *window = new GLFWWindow(width, height, graphicsApi);
	mWindows.push_back(static_cast<Window*>(window));

	// register events.
	glfwSetKeyCallback(window->mWindow, GLFWCallbacks::keyCallback);
	glfwSetMouseButtonCallback(window->mWindow, GLFWCallbacks::mouseButtonCallback);
	glfwSetCursorPosCallback(window->mWindow, GLFWCallbacks::mousePositionCallback);

	return static_cast<Window*>(window);
}

void GLFWWindowManager::processEvents()
{
	glfwPollEvents();
}

//-----------------------------------------------------------------------------
// GLFW Input callbacks.
//-----------------------------------------------------------------------------

namespace GLFWCallbacks
{
	void keyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods)
	{

	}

	void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{

	}

	void mousePositionCallback(GLFWwindow *window, double xPos, double yPos)
	{

	}
}