#ifndef _PLATFORM_WINDOWMANAGER_HPP_
#define _PLATFORM_WINDOWMANAGER_HPP_

#include <vector>
#include "core/singleton.hpp"
#include "platform/window.hpp"

/// A singleton class that manages window and input for the underneigth 
/// subsystems.
class WindowManager : public Singleton<WindowManager>
{
	friend class Singleton<WindowManager>;
protected:
	WindowManager();
	~WindowManager();
	WindowManager(const WindowManager &) = delete;
	WindowManager(const WindowManager &&) = delete;

public:
	Window* createWindow(int width, int height, Window::API graphicsApi = Window::API::eOPENGL);

	void destroyWindow(Window *window);

	Window* getPrimaryWindow() const;

	void processEvents();
private:
	std::vector<Window*> mWindows;
};

#endif
