//-----------------------------------------------------------------------------
// GLFWInputKeys.cpp
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

#include <GLFW/glfw3.h>
#include "platform/input/inputKeys.hpp"

// The Key API is mapped directly to GLFW.

namespace Input
{
	Key toKeyCode(int key)
	{
		return static_cast<Key>(key);
	}

	int toPlatformKeyCode(Key key)
	{
		return static_cast<int>(key);
	}

	KeyModifiers toKeyModifier(int mod)
	{
		int mods = KeyModifiers::eNONE;
		if (mod & GLFW_MOD_SHIFT)
			mods |= KeyModifiers::eSHIFT;
		if (mod & GLFW_MOD_CONTROL)
			mods |= KeyModifiers::eCTRL;
		if (mod & GLFW_MOD_ALT)
			mods |= KeyModifiers::eALT;
		if (mod & GLFW_MOD_SUPER)
			mods |= KeyModifiers::eSUPER;
		return static_cast<KeyModifiers>(mods);
	}

	int toPlatformModifier(KeyModifiers mod)
	{
		int mods = 0x0;
		if (mod & KeyModifiers::eSHIFT)
			mods |= GLFW_MOD_SHIFT;
		if (mod & KeyModifiers::eCTRL)
			mods |= GLFW_MOD_CONTROL;
		if (mod & KeyModifiers::eALT)
			mods |= GLFW_MOD_ALT;
		if (mod & KeyModifiers::eSUPER)
			mods |= GLFW_MOD_SUPER;
		return mods;
	}
}