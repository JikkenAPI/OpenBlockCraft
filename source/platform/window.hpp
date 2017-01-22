#ifndef _PLATFORM_WINDOW_H_
#define _PLATFORM_WINDOW_H_

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

	void setTitle(const char *title);

	GLFWwindow* getNativePtr() const { return mWindow;  }

private:
	GLFWwindow *mWindow;
};

#endif