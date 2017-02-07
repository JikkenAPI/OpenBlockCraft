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

#ifndef _SCENE_WORLD_BLOCK_HPP_
#define _SCENE_WORLD_BLOCK_HPP_

enum class RenderPass
{
	eGEOMETRY = 0,
	eTRANSLUCENT = 1
};

struct Block
{
	uint8_t id;
};

enum BlockType : uint8_t
{
	eAIR = 0,
	eGRASS,
	eDIRT,
	eCOBBLESTONE,
	eWATER
};

#endif