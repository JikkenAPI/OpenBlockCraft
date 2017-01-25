#include <cstdlib>
#include <string>
#include "platform/platform.hpp"
#include "platform/windowManager.hpp"
#include "platform/timer.hpp"

int main(int argc, const char **argv) 
{
	Platform::initSubSystems();

	Window* window = Platform::getWindowManager()->createWindow(1440, 900);
	Timer *timer = Platform::createTimer();
	while (!window->shouldClose())
	{
		timer->start();

		// show delta from last frame (value of timer)
		double fps = timer->getFPS();
		window->setTitle(std::to_string(static_cast<unsigned long>(fps)) + std::string(" fps"));
		
		window->swapBuffers();
		Platform::getWindowManager()->processEvents();
		timer->stop();
	}

	Platform::getWindowManager()->destroyWindow(window);
	Platform::destroyTimer(timer);
	Platform::cleanupSubSystems();
	return 0;
}