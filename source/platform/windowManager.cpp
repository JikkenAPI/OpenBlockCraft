#include <cassert>
#include "platform/windowManager.hpp"

WindowManager *gWindowManager = nullptr;

WindowManager* WindowManager::getSingleton()
{
	if (gWindowManager == nullptr)
		gWindowManager = new WindowManager();
	return gWindowManager;
}

void WindowManager::destroy()
{
	assert(gWindowManager != nullptr);
	delete gWindowManager;
	gWindowManager = nullptr;
}

WindowManager::WindowManager()
{

}

WindowManager::~WindowManager()
{
	// cleanup all windows
	for (Window *window : mWindows)
		delete window;
}

Window* WindowManager::createWindow(int width, int height, Window::API graphicsApi)
{
	Window *window = new Window(width, height, graphicsApi);
	mWindows.push_back(window);
	return window;
}

void WindowManager::destroyWindow(Window *window)
{
	// remove from window vector and delete the window
	auto position = std::find(mWindows.begin(), mWindows.end(), window);
	if (position != mWindows.end())
	{
		mWindows.erase(position);
		delete window;
		window = nullptr;
	}
}

Window* WindowManager::getPrimaryWindow() const
{
	// We need at least 1 window allocated!
	assert(mWindows.size() != 0);

	// The primary window is window 0. Always.
	return mWindows[0];
}

void WindowManager::processEvents()
{
	glfwPollEvents();
}