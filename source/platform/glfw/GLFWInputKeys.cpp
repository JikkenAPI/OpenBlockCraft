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