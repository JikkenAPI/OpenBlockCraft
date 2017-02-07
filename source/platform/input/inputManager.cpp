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