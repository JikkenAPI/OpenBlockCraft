#include <cassert>
#include "platform/glfw/GLFWWindowManager.hpp"
#include "platform/glfw/GLFWWindow.hpp"

GLFWWindowManager::GLFWWindowManager()
{

}

GLFWWindowManager::~GLFWWindowManager()
{

}

Window* GLFWWindowManager::createWindow(int width, int height, Window::API graphicsApi)
{
	Window *window = new GLFWWindow(width, height, graphicsApi);
	mWindows.push_back(window);
	return window;
}

void GLFWWindowManager::processEvents()
{
	glfwPollEvents();
}