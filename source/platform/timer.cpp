#include <cassert>
#include "platform/timer.hpp"

Timer::Timer() 
{
	mStart = 0;
	mDelta = 0;
	mStopped = true;
}

void Timer::start()
{
	// Can't call twice in a row.
	assert(mStopped);

	mStart = glfwGetTime() * 1000.0;
	mStopped = false;
}

void Timer::stop()
{
	// Can't call twice in a row.
	assert(!mStopped);

	double end = glfwGetTime() * 1000.0f;
	mDelta = end - mStart;
	mStopped = true;
}