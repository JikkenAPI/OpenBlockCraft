//-----------------------------------------------------------------------------
// inputEvents.hpp
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