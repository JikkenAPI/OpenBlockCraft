#include "platform/window.hpp"

Window::Window(int width, int height, API graphicsApi) 
{
	// set what kind of graphics we will need.
	if (graphicsApi == API::eOPENGL)
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	else
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// set up opengl context information
	if (graphicsApi == API::eOPENGL)
	{
		// We need at least a 3.3 core profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		// Request a debug context. At this time, MacOS does not have support
		// for debug contexts.
#if defined(_DEBUG) && !defined(__APPLE__)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	}

	mWindow = glfwCreateWindow(width, height, "", nullptr, nullptr);
	if (graphicsApi == API::eOPENGL)
		glfwMakeContextCurrent(mWindow);
}

Window::~Window() 
{
	glfwDestroyWindow(mWindow);
}