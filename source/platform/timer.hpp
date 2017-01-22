#ifndef _PLATFORM_TIMER_H_
#define _PLATFORM_TIMER_H_

#include <GLFW/glfw3.h>

class Timer
{
public:
	Timer();

	void start();
	void stop();

	inline double getDelta() const { return mDelta; }
	inline bool isStopped() const { return mStopped; }

private:
	double mStart;
	double mDelta;
	bool mStopped;
};

#endif