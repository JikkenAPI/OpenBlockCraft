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

#include <cassert>
#include <algorithm>
#include "platform/windowManager.hpp"

WindowManager::~WindowManager()
{
	// cleanup all windows
	for (Window *window : mWindows)
		delete window;
}

void WindowManager::destroyWindow(Window *window)
{
	// remove from window vector and delete the window
	auto position = std::find(mWindows.begin(), mWindows.end(), window);
	if (position != mWindows.end())
	{
		mWindows.erase(position);
		delete window;
		window = nullptr;
	}
}

Window* WindowManager::getPrimaryWindow() const
{
	// We need at least 1 window allocated!
	assert(mWindows.size() != 0);

	// The primary window is window 0. Always.
	return mWindows[0];
}
