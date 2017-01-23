#ifndef _PLATFORM_WINDOW_H_
#define _PLATFORM_WINDOW_H_

#include <string>
#include <GLFW/glfw3.h>

class Window 
{
	// Only the WindowManager class can allocate/deallocate Window
	// objects.
	friend class WindowManager;

public:
	enum class API
	{
		eNONE,
		eOPENGL
	};

	// Restriction of allocation.
private:
	Window(int width, int height, API);
	~Window();

	// no copy constructor
	Window(const Window &window) = delete;

public:
	void setTitle(const std::string &title);

	bool shouldClose() const;

	void swapBuffers();

private:
	GLFWwindow *mWindow;
};

#endif