//-----------------------------------------------------------------------------
// OpenBlockCraft -- A 3D block based game
// Copyright(C) 2017 Jeff Hutchinson
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>
//-----------------------------------------------------------------------------

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