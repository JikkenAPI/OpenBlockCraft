//-----------------------------------------------------------------------------
// GLFWWindowManager.hpp
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

#ifndef _PLATFORM_GLFW_GLFWWINDOWMANAGER_HPP_
#define _PLATFORM_GLFW_GLFWWINDOWMANAGER_HPP_

#include "platform/windowManager.hpp"

/// A singleton class that manages window and input for the underneigth 
/// subsystems.
class GLFWWindowManager : public WindowManager, public Singleton<GLFWWindowManager>
{
	friend class Singleton<GLFWWindowManager>;

protected:
	GLFWWindowManager();
	virtual ~GLFWWindowManager() override;
	GLFWWindowManager(const GLFWWindowManager &) = delete;
	GLFWWindowManager(const GLFWWindowManager &&) = delete;

public:
	virtual Window* createWindow(int width, int height, Window::API graphicsApi = Window::API::eOPENGL) override;

	virtual void processEvents() override;
};

#endif
