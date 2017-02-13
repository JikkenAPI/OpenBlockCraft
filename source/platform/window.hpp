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

#ifndef _PLATFORM_WINDOW_H_
#define _PLATFORM_WINDOW_H_

#include <string>
#include "platform/input/iInputListener.hpp"

class Window : public IInputListener
{
	// Only the WindowManager class can allocate/deallocate Window
	// objects.
	friend class WindowManager;
public:
	enum class API
	{
		eNONE,
		eOPENGL,
		eVULKAN
	};

public:
	virtual void onKeyPressEvent(const KeyPressEventData &data) override 
	{
		if (data.keyState == Input::KeyState::ePRESSED)
		{
			if (data.key == Input::Key::eESCAPE)
				toggleCursor();
			else if (data.key == Input::Key::eF1)
				toggleVerticalSync();
		}
	}

	virtual void setTitle(const std::string &title) = 0;

	virtual bool shouldClose() const = 0;

	virtual void toggleCursor() = 0;

	virtual void toggleVerticalSync() = 0;

protected:
	bool mVsync = false;
};

#endif