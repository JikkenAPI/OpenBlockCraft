#ifndef _PLATFORM_PLATFORM_H_
#define _PLATFORM_PLATFORM_H_

#include <cassert>
#include "platform/timer.hpp"
#include "platform/windowManager.hpp"

namespace Platform
{
	void initSubSystems();

	void cleanupSubSystems();

	Timer* createTimer();

	void destroyTimer(Timer *timer);

	WindowManager* getWindowManager();
}

#endif