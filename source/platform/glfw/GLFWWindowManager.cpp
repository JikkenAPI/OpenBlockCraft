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
#include "platform/glfw/GLFWWindowManager.hpp"
#include "platform/glfw/GLFWWindow.hpp"
#include "platform/input/inputManager.hpp"

GLFWWindowManager::GLFWWindowManager()
{

}

GLFWWindowManager::~GLFWWindowManager()
{

}

IWindow* GLFWWindowManager::createWindow(int width, int height)
{
	GLFWWindow *window = new GLFWWindow(width, height);
	mWindows.push_back(static_cast<IWindow*>(window));

	// register events.
	glfwSetKeyCallback(window->mWindow, GLFWCallbacks::keyCallback);
	glfwSetMouseButtonCallback(window->mWindow, GLFWCallbacks::mouseButtonCallback);
	glfwSetCursorPosCallback(window->mWindow, GLFWCallbacks::mousePositionCallback);
	glfwSetFramebufferSizeCallback(window->mWindow, GLFWCallbacks::framebufferResizeCallback);

	return static_cast<IWindow*>(window);
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
		// We don't care about repeat events for now.
		if (action == GLFW_REPEAT)
			return;

		KeyPressEventData data;
		data.key = Input::toKeyCode(key);
		data.modifiers = Input::toKeyModifier(mods);
		data.keyState = (action == GLFW_PRESS) ? Input::KeyState::ePRESSED : Input::KeyState::eRELEASED;

		InputManager::get()->fireCallback(InputEventType::eKEYPRESSEVENT, data);
	}

	void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{
		MouseClickData data;
		data.state = (action == GLFW_PRESS) ? Input::MouseButtonState::ePRESSED : Input::MouseButtonState::eRELEASED;
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			data.button = Input::MouseButton::eLEFT;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			data.button = Input::MouseButton::eRIGHT;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			data.button = Input::MouseButton::eMIDDLE;
			break;
		default:
			// We don't care about any other states.
			return;
		}
		
		InputManager::get()->fireCallback(InputEventType::eMOUSEBUTTONEVENT, data);
	}

	void mousePositionCallback(GLFWwindow *window, double xPos, double yPos)
	{
		MousePositionData data;
		data.x = xPos;
		data.y = yPos;

		InputManager::get()->fireCallback(InputEventType::eMOUSEMOVEMENTEVENT, data);
	}

	void framebufferResizeCallback(GLFWwindow *window, int width, int height)
	{
		Jikken::resize(width, height);
	}
}