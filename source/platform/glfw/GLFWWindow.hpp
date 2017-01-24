#ifndef _PLATFORM_GLFW_GLFWWINDOW_H_
#define _PLATFORM_GLFW_GLFWWINDOW_H_

#include <string>
#include <GLFW/glfw3.h>

#include "platform/window.hpp"

class GLFWWindow : public Window 
{
	// Only the WindowManager class can allocate/deallocate Window
	// objects.
	friend class GLFWWindowManager;

	// Restriction of allocation.
private:
	GLFWWindow(int width, int height, API);
	~GLFWWindow();

	// no copy constructor
	GLFWWindow(const GLFWWindow &window) = delete;

public:
	virtual void setTitle(const std::string &title) override;

	virtual bool shouldClose() const override;

	virtual void swapBuffers() override;

private:
	GLFWwindow *mWindow;
};

#endif