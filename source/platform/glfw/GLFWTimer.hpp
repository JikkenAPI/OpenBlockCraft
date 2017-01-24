#ifndef _PLATFORM_GLFW_GLFWTIMER_H_
#define _PLATFORM_GLFW_GLFWTIMER_H_

#include "platform/timer.hpp"
#include <GLFW/glfw3.h>

class GLFWTimer : public Timer
{
public:
	GLFWTimer();

	virtual void start() override;
	virtual void stop() override;
};

#endif