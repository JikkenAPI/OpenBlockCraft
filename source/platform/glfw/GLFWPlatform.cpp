#include <cassert>
#include <GLFW/glfw3.h>

#include "platform/glfw/GLFWTimer.hpp"
#include "platform/glfw/GLFWWindowManager.hpp"

namespace Platform
{
	void initSubSystems()
	{
		// Init glfw
		if (glfwInit() != GLFW_TRUE)
		{
			assert(false);
		}
	}

	void cleanupSubSystems()
	{
		// destroy window manager
		GLFWWindowManager::destroy();

		// destroy glfw
		glfwTerminate();
	}

	Timer* createTimer()
	{
		return new GLFWTimer();
	}

	void destroyTimer(Timer *timer)
	{
		delete timer;
		timer = nullptr;
	}

	WindowManager* getWindowManager()
	{
		return GLFWWindowManager::get();
	}
}