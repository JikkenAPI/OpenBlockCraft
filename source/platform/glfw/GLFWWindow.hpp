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

#ifndef _PLATFORM_GLFW_GLFWWINDOW_H_
#define _PLATFORM_GLFW_GLFWWINDOW_H_

#include <string>
#include <GLFW/glfw3.h>

#include "platform/window.hpp"

class GLFWWindow : public Window 
{
	// Only the WindowManager class can allocate/deallocate Window
	// objects.
	friend class GLFWWindowManager;

	// Restriction of allocation.
private:
	GLFWWindow(int width, int height);
	~GLFWWindow();

	// no copy constructor
	GLFWWindow(const GLFWWindow &window) = delete;

public:
	virtual void setTitle(const std::string &title) override;

	virtual bool shouldClose() const override;

	virtual void toggleCursor() override;

	virtual Jikken::NativeWindowData getJikkenNativeWindowData() override;

private:
	GLFWwindow *mWindow;
	bool mCursorShowing = true;
};

#endif