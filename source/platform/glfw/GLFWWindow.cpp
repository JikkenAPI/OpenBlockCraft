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
#include "platform/glfw/GLFWWindow.hpp"
#include <GLFW/glfw3native.h>


GLFWWindow::GLFWWindow(int width, int height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	mWindow = glfwCreateWindow(width, height, "", nullptr, nullptr);
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

void GLFWWindow::toggleCursor()
{
	mCursorShowing = !mCursorShowing;
	glfwSetInputMode(mWindow, GLFW_CURSOR, mCursorShowing ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

Jikken::NativeWindowData GLFWWindow::getJikkenNativeWindowData()
{
	Jikken::NativeWindowData wd = {};
#ifdef _WIN32
	wd.handle = glfwGetWin32Window(mWindow);
#elif defined __APPLE__
	wd.handle = glfwGetCocoaWindow(mWindow);
#else
	wd.handle = glfwGetX11Window(mWindow);
	wd.display = glfwGetX11Display();
#endif
	return wd;
}