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
