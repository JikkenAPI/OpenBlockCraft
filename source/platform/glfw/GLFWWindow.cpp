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
	glfwGetFramebufferSize(mWindow, &wd.fbWidth, &wd.fbHeight);
#ifdef _WIN32
	wd.handle = glfwGetWin32Window(mWindow);
#elif defined __APPLE__
	wd.handle = glfwGetCocoaWindow(mWindow);
#else
	wd.handle = (void*)(uintptr_t)glfwGetX11Window(mWindow);
	wd.display = glfwGetX11Display();
#endif
	return wd;
}