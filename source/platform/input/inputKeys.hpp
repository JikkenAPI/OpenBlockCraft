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

#ifndef _PLATFORM_INPUT_INPUTKEYS_HPP_
#define _PLATFORM_INPUT_INPUTKEYS_HPP_

/// Methods that convert between platform specific key codes and our API.
namespace Input
{
	/// Key enum that represents key values.
	/// Keys align with the GLFW API.
	/// See: http://www.glfw.org/docs/latest/group__keys.html
	enum class Key
	{
		eUNKNOWN = -1,
		eSPACE = 32,
		eAPOSTROPHE = 39,
		eCOMMA = 44,
		eMINUS = 45,
		ePERIOD = 46,
		eSLASH = 47,
		e0 = 48,
		e1 = 49,
		e2 = 50,
		e3 = 51,
		e4 = 52,
		e5 = 53,
		e6 = 54,
		e7 = 55,
		e8 = 56,
		e9 = 57,
		eSEMICOLON = 59,
		eEQUAL = 61,
		eA = 65,
		eB = 66,
		eC = 67,
		eD = 68,
		eE = 69,
		eF = 70,
		eG = 71,
		eH = 72,
		eI = 73,
		eJ = 74,
		eK = 75,
		eL = 76,
		eM = 77,
		eN = 78,
		eO = 79,
		eP = 80,
		eQ = 81,
		eR = 82,
		eS = 83,
		eT = 84,
		eU = 85,
		eV = 86,
		eW = 87,
		eX = 88,
		eY = 89,
		eZ = 90,
		eLEFTBRACKET = 91,
		eBACKSLASH = 92,
		eRIGHTBRACKET = 93,
		eGRAVEACCENT = 96,
		eWORLD1 = 161,
		eWORLD2 = 162,
		eESCAPE = 256,
		eENTER = 257,
		eTAB = 258,
		eBACKSPACE = 259,
		eINSERT = 260,
		eDELETE = 261,
		eRIGHT = 262,
		eLEFT = 263,
		eDOWN = 264,
		eUP = 265,
		ePAGEUP = 266,
		ePAGEDOWN = 267,
		eHOME = 268,
		eEND = 269,
		eCAPSLOCK = 280,
		eSCROLLLOCK = 281,
		eNUMLOCK = 282,
		ePRINTSCREEN = 283,
		ePAUSE = 284,
		eF1 = 290,
		eF2 = 291,
		eF3 = 292,
		eF4 = 293,
		eF5 = 294,
		eF6 = 295,
		eF7 = 296,
		eF8 = 297,
		eF9 = 298,
		eF10 = 299,
		eF11 = 300,
		eF12 = 301,
		eF13 = 302,
		eF14 = 303,
		eF15 = 304,
		eF16 = 305,
		eF17 = 306,
		eF18 = 307,
		eF19 = 308,
		eF20 = 309,
		eF21 = 310,
		eF22 = 311,
		eF23 = 312,
		eF24 = 313,
		eF25 = 314,
		eKP0 = 320,
		eKP1 = 321,
		eKP2 = 322,
		eKP3 = 323,
		eKP4 = 324,
		eKP5 = 325,
		eKP6 = 326,
		eKP7 = 327,
		eKP8 = 328,
		eKP9 = 329,
		eKPDECIMAL = 330,
		eKPDIVIDE = 331,
		eKPMULTIPLY = 322,
		eKPSUBTRACT = 333,
		eKPADD = 334,
		eKPENTER = 335,
		eKPEQUAL = 336,
		eLEFTSHIFT = 340,
		eLEFTCTRL = 341,
		eLEFTALT = 342,
		eRIGHTSHIFT = 344,
		eRIGHTCTRL = 345,
		eRIGHTALT = 346,
		eRIGHTSUPER = 347,
		eMENU = 348
	};

	/// Key State
	enum class KeyState
	{
		ePRESSED = 0,
		eRELEASED = 1
	};

	/// Key modifiers
	/// Match GLFW.
	enum KeyModifiers
	{
		eNONE = 0x0,
		eSHIFT = 0x1,
		eCTRL = 0x2,
		eALT = 0x4,
		eSUPER = 0x8
	};

	/// Mouse button state
	enum class MouseButtonState
	{
		ePRESSED = 0,
		eRELEASED = 1
	};

	/// Mouse buttons to press
	enum class MouseButton
	{
		eLEFT = 0,
		eRIGHT = 1,
		eMIDDLE = 2
	};

	/// Converts from a platform specific keycode to our keycode.
	/// @param key The platform specific key represented as a 32bit integer.
	/// @return Key The key enumeration for the specified key.
	Key toKeyCode(int key);

	/// Converts from our keycode system to the platform specific keycode.
	/// @param key The key enumeration for the specified key.
	/// @return int The platform specific key represented as a 32bit integer.
	int toPlatformKeyCode(Key key);

	/// Converts from a platform specific key modifier to our key modifier.
	/// @param mod The platform specific modifier bitflag.
	/// @return KeyModifiers a bitflag representing key modifiers.
	KeyModifiers toKeyModifier(int mod);

	/// Converts from our key modifier system to the platform specific system.
	/// @param mod The modifier bitflag.
	/// @return int A bitflag representing the platform specific modifiers.
	int toPlatformModifier(KeyModifiers mod);
}

#endif