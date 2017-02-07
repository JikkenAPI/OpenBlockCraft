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

#ifndef _PLATFORM_INPUT_IINPUTLISTENER_HPP_
#define _PLATFORM_INPUT_IINPUTLISTENER_HPP_

#include "platform/input/inputEvents.hpp"

/// An Interface that allows subscribing to the InputManager.
class IInputListener
{
public:
	virtual void onKeyPressEvent(const KeyPressEventData &data) {}

	virtual void onMouseClickEvent(const MouseClickData &data) {}

	virtual void onMouseMoveEvent(const MousePositionData &data) {}
};

#endif