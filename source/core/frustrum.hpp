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

#ifndef _CORE_FRUSTRUM_HPP_
#define _CORE_FRUSTRUM_HPP_

#include <glm/glm.hpp>

class Frustrum
{
public:
	enum PlaneID
	{
		eRIGHT = 0,
		eLEFT,
		eBOTTOM,
		eTOP,
		eFAR,
		eNEAR
	};

	Frustrum();
	~Frustrum();

	void setVP(const glm::mat4 &view, const glm::mat4 &proj);

	const glm::vec4 & getNormalizedPlane(PlaneID index) const;

	bool checkPoint(const glm::vec3 &point) const;

	bool checkSphere(const glm::vec3 &point, float radius) const;

	/// Checks if the cube is within the frustrum.
	/// @param center The center point of the cube.
	/// @param length The length of the side of the cube to the center point.
	/// Note: This is labeled as FAST, as the author of the algorithm
	/// states that it can occasionally cause false positives, rendering more
	/// than necessary. However it is computationally less expensive.
	bool checkCubeFAST(const glm::vec3 &center, const glm::vec3 &length) const;

private:
	/// 6 Normalized planes that represent the frustrum.
	glm::vec4 mPlanes[6];
};

#endif