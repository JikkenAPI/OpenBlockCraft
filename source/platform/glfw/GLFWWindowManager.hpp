#ifndef _PLATFORM_GLFW_GLFWWINDOWMANAGER_HPP_
#define _PLATFORM_GLFW_GLFWWINDOWMANAGER_HPP_

#include "platform/windowManager.hpp"

/// A singleton class that manages window and input for the underneigth 
/// subsystems.
class GLFWWindowManager : public WindowManager, public Singleton<GLFWWindowManager>
{
	friend class Singleton<GLFWWindowManager>;

protected:
	GLFWWindowManager();
	virtual ~GLFWWindowManager() override;
	GLFWWindowManager(const GLFWWindowManager &) = delete;
	GLFWWindowManager(const GLFWWindowManager &&) = delete;

public:
	virtual Window* createWindow(int width, int height, Window::API graphicsApi = Window::API::eOPENGL) override;

	virtual void processEvents() override;
};

#endif
