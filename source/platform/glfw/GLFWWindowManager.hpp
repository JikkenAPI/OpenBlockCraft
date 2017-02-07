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

#ifndef _PLATFORM_GLFW_GLFWWINDOWMANAGER_HPP_
#define _PLATFORM_GLFW_GLFWWINDOWMANAGER_HPP_

#include "platform/windowManager.hpp"

struct GLFWwindow;

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

namespace GLFWCallbacks
{
	void keyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods);

	void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

	void mousePositionCallback(GLFWwindow *window, double xPos, double yPos);
}

#endif
