#include <cstdlib>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <open-simplex-noise.h>
#include <glm/glm.hpp>

#include "platform/window.hpp"
#include "platform/timer.hpp"

int main(int argc, const char **argv) 
{
	glfwInit();

	Window window(1440, 900);
	Timer timer;
	while (1)
	{
		timer.start();
		if (glfwWindowShouldClose(window.getNativePtr()))
			break;

		// show delta from last frame (value of timer)
		double fps = 1000.0 / timer.getDelta();
		glfwSetWindowTitle(window.getNativePtr(), (std::to_string(static_cast<unsigned long>(fps)) + std::string(" fps")).c_str());
		
		glfwSwapBuffers(window.getNativePtr());
		glfwPollEvents();
		timer.stop();
	}

	glfwTerminate();
	return 0;
}