#ifndef _PLATFORM_WINDOWMANAGER_HPP_
#define _PLATFORM_WINDOWMANAGER_HPP_

#include <vector>
#include "core/singleton.hpp"
#include "platform/window.hpp"

/// A singleton class that manages window and input for the underneigth 
/// subsystems.
class WindowManager
{
protected:
	virtual ~WindowManager();

public:
	virtual Window* createWindow(int width, int height, Window::API graphicsApi = Window::API::eOPENGL) = 0;

	virtual void processEvents() = 0;

	void destroyWindow(Window *window);

	Window* getPrimaryWindow() const;

protected:
	std::vector<Window*> mWindows;
};

#endif
