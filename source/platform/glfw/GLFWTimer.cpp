#include <cassert>
#include "platform/glfw/GLFWTimer.hpp"

GLFWTimer::GLFWTimer() 
{
}

void GLFWTimer::start()
{
	// Can't call twice in a row.
	assert(mStopped);

	mStart = glfwGetTime() * 1000.0;
	mStopped = false;
}

void GLFWTimer::stop()
{
	// Can't call twice in a row.
	assert(!mStopped);

	double end = glfwGetTime() * 1000.0f;
	mDelta = end - mStart;
	mStopped = true;
}