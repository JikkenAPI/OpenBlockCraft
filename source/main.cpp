#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <open-simplex-noise.h>
#include <glm/glm.hpp>

#include "platform/window.hpp"

int main(int argc, const char **argv) 
{
	glfwInit();

	Window window(1440, 900);
	while (1)
	{
		if (glfwWindowShouldClose(window.getNativePtr()))
			break;
		
		glfwSwapBuffers(window.getNativePtr());
	}

	glfwTerminate();
	return 0;
}