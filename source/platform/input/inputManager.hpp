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

#ifndef _PLATFORM_INPUT_INPUTMANAGER_HPP_
#define _PLATFORM_INPUT_INPUTMANAGER_HPP_

#include <array>
#include <vector>
#include "core/singleton.hpp"
#include "platform/input/inputEvents.hpp"
#include "platform/input/iInputListener.hpp"
#include "platform/timer.hpp"

class InputManager : public Singleton<InputManager>
{
	friend class Singleton<InputManager>;

	/// The type that holds the objects subscribed to each type of event.
	/// The container for each type of subscription will hold the object.
	typedef std::array<std::vector<IInputListener*>, InputEventType::eEVENTCOUNT> Container;

protected:
	InputManager();
	~InputManager();
	InputManager(const InputManager &) = delete;
	InputManager(const InputManager &&) = delete;

public:

	void setTimer(const Timer *timer);

	void subscribe(IInputListener *obj, InputEventType eventType);

	void fireCallback(InputEventType eventType, IInputEventData &data);

protected:
	Container mSubscribers;

	const Timer *mTimer;
};

#endif
