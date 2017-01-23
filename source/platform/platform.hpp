#ifndef _PLATFORM_PLATFORM_H_
#define _PLATFORM_PLATFORM_H_

#include <cassert>
#include <GLFW/glfw3.h>

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
		// destroy glfw
		glfwTerminate();
	}
}

#endif