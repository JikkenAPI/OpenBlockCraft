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

public:
	virtual void setTitle(const std::string &title) = 0;

	virtual bool shouldClose() const = 0;

	virtual void swapBuffers() = 0;
};

#endif