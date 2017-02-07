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

#ifndef _PLATFORM_INPUT_INPUTEVENTS_HPP_
#define _PLATFORM_INPUT_INPUTEVENTS_HPP_

#include "inputKeys.hpp"

enum InputEventType : size_t
{
	eKEYPRESSEVENT = 0,
	eMOUSEBUTTONEVENT = 1,
	eMOUSEMOVEMENTEVENT = 2,

	eEVENTCOUNT
};

/// Interface that is used as a common base for all input event data.
struct IInputEventData
{
	double mDeltaTime = 0.0;
};

struct KeyPressEventData : public IInputEventData
{
	Input::KeyState keyState;
	Input::Key key;
	Input::KeyModifiers modifiers;
};

struct MouseClickData : public IInputEventData
{
	Input::MouseButton button;
	Input::MouseButtonState state;
};

struct MousePositionData : public IInputEventData
{
	double x;
	double y;
};

#endif