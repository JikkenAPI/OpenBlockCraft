//-----------------------------------------------------------------------------
// windowManager.cpp
//
// Copyright(c) 2017 Jeff Hutchinson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-----------------------------------------------------------------------------

#include <cassert>
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