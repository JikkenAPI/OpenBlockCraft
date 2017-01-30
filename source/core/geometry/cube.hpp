//-----------------------------------------------------------------------------
// cube.hpp
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

#ifndef _CORE_GEOMETRY_CUBE_HPP_
#define _CORE_GEOMETRY_CUBE_HPP_

#include <glm/glm.hpp>

glm::vec4 sCubeFaceNormals[6] = {
	glm::vec4(0.0f, 0.0f, -1.0f, 1.0f),
	glm::vec4(0.0f,  0.0f,  1.0f, 1.0f),
	glm::vec4(-1.0f,  0.0f,  0.0f, 1.0f),
	glm::vec4(1.0f,  0.0f,  0.0f, 1.0f),
	glm::vec4(0.0f, -1.0f,  0.0f, 1.0f),
	glm::vec4(0.0f,  1.0f,  0.0f, 1.0f)
};

struct CubeVert
{
	glm::vec3 pos;
	int normal;
};

CubeVert sCubeFaceVertices[6][4] = {
	{
		// Positions              // Normals Index        
		{ glm::vec3(-0.5f, -0.5f, -0.5f), 0 },
		{ glm::vec3( 0.5f, -0.5f, -0.5f), 0 },
		{ glm::vec3( 0.5f,  0.5f, -0.5f), 0 },
		{ glm::vec3(-0.5f,  0.5f, -0.5f), 0 }
	},
	{
		{ glm::vec3(-0.5f, -0.5f,  0.5f), 1 },
		{ glm::vec3( 0.5f, -0.5f,  0.5f), 1 },
		{ glm::vec3( 0.5f,  0.5f,  0.5f), 1 },
		{ glm::vec3(-0.5f,  0.5f,  0.5f), 1 }
	},
	{	
		{ glm::vec3(-0.5f,  0.5f,  0.5f), 2 },
		{ glm::vec3(-0.5f,  0.5f, -0.5f), 2 },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), 2 },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), 2 }
	},
	{
		{ glm::vec3(0.5f,  0.5f,  0.5f),  3},
		{ glm::vec3(0.5f,  0.5f, -0.5f),  3},
		{ glm::vec3(0.5f, -0.5f, -0.5f),  3},
		{ glm::vec3(0.5f, -0.5f,  0.5f),  3}
	},
	{
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  4},
		{ glm::vec3( 0.5f, -0.5f, -0.5f),  4},
		{ glm::vec3( 0.5f, -0.5f,  0.5f),  4},
		{ glm::vec3(-0.5f, -0.5f,  0.5f),  4}
	},
	{
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  5},
		{ glm::vec3( 0.5f,  0.5f, -0.5f),  5},
		{ glm::vec3( 0.5f,  0.5f,  0.5f),  5},
		{ glm::vec3(-0.5f,  0.5f,  0.5f),  5}
	}
};

short sCubeFaceIndices[6] = {
	0,1,2,2,3,0
};

#endif