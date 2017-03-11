//-----------------------------------------------------------------------------
// MIT License
// 
// Copyright (c) 2017 Jeff Hutchinson
// Copyright (c) 2017 Tim Barnes
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-----------------------------------------------------------------------------

#include <cassert>
#include <algorithm>
#include "platform/input/inputManager.hpp"

InputManager::InputManager()
{
	mTimer = nullptr;
}

InputManager::~InputManager()
{

}

void InputManager::setTimer(const Timer *timer)
{
	mTimer = timer;
}

void InputManager::subscribe(IInputListener *obj, InputEventType eventType)
{
	// Make sure that we can only subscribe once to the event type.
	const auto &vec = mSubscribers[eventType];
	auto pos = std::find(vec.begin(), vec.end(), obj);
	if (pos == vec.end())
	{
		// Subscribe.
		mSubscribers[eventType].push_back(obj);
	}
#ifdef _DEBUG
	else
	{
		// We don't want to add more than once!
		assert(false);
	}
#endif
}

void InputManager::fireCallback(InputEventType eventType, IInputEventData &data)
{
	// set delta time for event.
	data.mDeltaTime = (mTimer != nullptr) ? mTimer->getDelta() : 0.0;

	const auto &vec = mSubscribers[eventType];
	switch (eventType)
	{
		case InputEventType::eKEYPRESSEVENT:
			for (IInputListener *listener : vec)
				listener->onKeyPressEvent(reinterpret_cast<const KeyPressEventData&>(data));
			break;
		case InputEventType::eMOUSEBUTTONEVENT:
			for (IInputListener *listener : vec)
				listener->onMouseClickEvent(reinterpret_cast<const MouseClickData&>(data));
			break;
		case InputEventType::eMOUSEMOVEMENTEVENT:
			for (IInputListener *listener : vec)
				listener->onMouseMoveEvent(reinterpret_cast<const MousePositionData&>(data));
			break;
	}
}
