#include <cassert>
#include "platform/windowManager.hpp"

WindowManager::~WindowManager()
{
	// cleanup all windows
	for (Window *window : mWindows)
		delete window;
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