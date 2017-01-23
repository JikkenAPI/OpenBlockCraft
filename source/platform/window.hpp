#ifndef _PLATFORM_WINDOW_H_
#define _PLATFORM_WINDOW_H_

#include <string>
#include <GLFW/glfw3.h>

class Window 
{
	enum class API
	{
		eNONE,
		eOPENGL
	};

public:
	Window(int width, int height, API = API::eOPENGL);
	~Window();

	void setTitle(const std::string &title);

	bool shouldClose() const;

	void swapBuffers();

private:
	GLFWwindow *mWindow;
};

#endif