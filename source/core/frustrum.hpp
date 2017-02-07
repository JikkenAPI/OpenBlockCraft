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