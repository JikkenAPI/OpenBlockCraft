#ifndef _PLATFORM_TIMER_H_
#define _PLATFORM_TIMER_H_

class Timer
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;

	inline double getDelta() const { return mDelta; }

	inline bool isStopped() const { return mStopped; }

	inline double getFPS() const 
	{ 
		if (mDelta == 0.0)
			return 0.0;
		return 1000.0 / mDelta;
	}

protected:
	double mStart = 0.0;
	double mDelta = 0.0;
	bool mStopped = true;
};

#endif