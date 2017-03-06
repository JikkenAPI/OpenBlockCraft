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

#ifndef _PLATFORM_WINDOWMANAGER_HPP_
#define _PLATFORM_WINDOWMANAGER_HPP_

#include <vector>
#include "core/singleton.hpp"
#include "platform/window.hpp"

/// A singleton class that manages window and input for the underneigth 
/// subsystems.
class WindowManager
{
protected:
	virtual ~WindowManager();

public:
	virtual IWindow* createWindow(int width, int height) = 0;

	virtual void processEvents() = 0;

	void destroyWindow(IWindow *window);

	IWindow* getPrimaryWindow() const;

protected:
	std::vector<IWindow*> mWindows;
};

#endif
