#include <cstdlib>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <open-simplex-noise.h>
#include <glm/glm.hpp>

#include "platform/platform.hpp"
#include "platform/windowManager.hpp"
#include "platform/timer.hpp"

int main(int argc, const char **argv) 
{
	Platform::initSubSystems();

	Window* window = WindowManager::get()->createWindow(1440, 900);
	Timer timer;
	while (!window->shouldClose())
	{
		timer.start();

		// show delta from last frame (value of timer)
		double fps = 1000.0 / timer.getDelta();
		window->setTitle(std::to_string(static_cast<unsigned long>(fps)) + std::string(" fps"));
		
		window->swapBuffers();
		WindowManager::get()->processEvents();
		timer.stop();
	}

	WindowManager::get()->destroyWindow(window);
	WindowManager::destroy();
	Platform::cleanupSubSystems();
	return 0;
}