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

#ifndef _CORE_GEOMETRY_CUBE_HPP_
#define _CORE_GEOMETRY_CUBE_HPP_

#include <glm/glm.hpp>

static glm::vec4 sCubeFaceNormals[6] = {
	glm::vec4(0.0f, 0.0f, -1.0f, 1.0f),
	glm::vec4(0.0f,  0.0f,  1.0f, 1.0f),
	glm::vec4(-1.0f,  0.0f,  0.0f, 1.0f),
	glm::vec4(1.0f,  0.0f,  0.0f, 1.0f),
	glm::vec4(0.0f, -1.0f,  0.0f, 1.0f),
	glm::vec4(0.0f,  1.0f,  0.0f, 1.0f)
};

enum CubeSides : int
{
	eBACK = 0,
	eFRONT,
	eLEFT,
	eRIGHT,
	eDOWN,
	eUP
};

struct CubeVert
{
	glm::vec3 pos;
	struct
	{
		uint16_t normal;
		uint16_t textureID = 0;
	};
};

struct InternalCubeVert
{
	glm::vec3 pos;
	int normal;
};

static InternalCubeVert sCubeFaceVertices[6][4] = {
	// BACK
	{
		// Positions              // Normals Index        
		{ glm::vec3(-0.5f, -0.5f, -0.5f), 0 },
		{ glm::vec3(-0.5f,  0.5f, -0.5f), 0 },
		{ glm::vec3( 0.5f,  0.5f, -0.5f), 0 },
		{ glm::vec3(0.5f, -0.5f, -0.5f),  0 }
	},
	// FRONT
	{
		{ glm::vec3(-0.5f, -0.5f,  0.5f), 1 },
		{ glm::vec3( 0.5f, -0.5f,  0.5f), 1 },
		{ glm::vec3( 0.5f,  0.5f,  0.5f), 1 },
		{ glm::vec3(-0.5f,  0.5f,  0.5f), 1 }
	},
	// LEFT
	{	
		{ glm::vec3(-0.5f,  0.5f,  0.5f), 2 },
		{ glm::vec3(-0.5f,  0.5f, -0.5f), 2 },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), 2 },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), 2 }
	},
	// RIGHT
	{
		{ glm::vec3(0.5f,  0.5f,  0.5f),  3 },
		{ glm::vec3(0.5f, -0.5f,  0.5f),  3 },
		{ glm::vec3(0.5f, -0.5f, -0.5f),  3 },
		{ glm::vec3(0.5f,  0.5f, -0.5f),  3 }
	},
	// DOWN
	{
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  4 },
		{ glm::vec3( 0.5f, -0.5f, -0.5f),  4 },
		{ glm::vec3( 0.5f, -0.5f,  0.5f),  4 },
		{ glm::vec3(-0.5f, -0.5f,  0.5f),  4 }
	},
	// UP
	{
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  5 },
		{ glm::vec3(-0.5f,  0.5f,  0.5f),  5 },
		{ glm::vec3( 0.5f,  0.5f,  0.5f),  5 },
		{ glm::vec3(0.5f,  0.5f, -0.5f),   5 }
	}
};

static short sCubeFaceIndices[6] = {
	0,1,2,2,3,0
};

#endif