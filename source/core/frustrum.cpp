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

#include "core/frustrum.hpp"

Frustrum::Frustrum()
{
	for (int i = 0; i < 6; ++i)
		mPlanes[i] = glm::vec4(0.0f);
}

Frustrum::~Frustrum()
{

}

void Frustrum::setVP(const glm::mat4 &view, const glm::mat4 &proj)
{
	glm::mat4 pv = proj * view;
	float *clip = &pv[0][0];

	// Set each plane.
	mPlanes[PlaneID::eRIGHT].x = clip[3] - clip[0];
	mPlanes[PlaneID::eRIGHT].y = clip[7] - clip[4];
	mPlanes[PlaneID::eRIGHT].z = clip[11] - clip[8];
	mPlanes[PlaneID::eRIGHT].w = clip[15] - clip[12];

	mPlanes[PlaneID::eLEFT].x = clip[3] + clip[0];
	mPlanes[PlaneID::eLEFT].y = clip[7] + clip[4];
	mPlanes[PlaneID::eLEFT].z = clip[11] + clip[8];
	mPlanes[PlaneID::eLEFT].w = clip[15] + clip[12];

	mPlanes[PlaneID::eBOTTOM].x = clip[3] + clip[1];
	mPlanes[PlaneID::eBOTTOM].y = clip[7] + clip[5];
	mPlanes[PlaneID::eBOTTOM].z = clip[11] + clip[9];
	mPlanes[PlaneID::eBOTTOM].w = clip[15] + clip[13];

	mPlanes[PlaneID::eTOP].x = clip[3] - clip[1];
	mPlanes[PlaneID::eTOP].y = clip[7] - clip[5];
	mPlanes[PlaneID::eTOP].z = clip[11] - clip[9];
	mPlanes[PlaneID::eTOP].w = clip[15] - clip[13];

	mPlanes[PlaneID::eFAR].x = clip[3] - clip[2];
	mPlanes[PlaneID::eFAR].y = clip[7] - clip[6];
	mPlanes[PlaneID::eFAR].z = clip[11] - clip[10];
	mPlanes[PlaneID::eFAR].w = clip[15] - clip[14];

	mPlanes[PlaneID::eNEAR].x = clip[3] + clip[2];
	mPlanes[PlaneID::eNEAR].y = clip[7] + clip[6];
	mPlanes[PlaneID::eNEAR].z = clip[11] + clip[10];
	mPlanes[PlaneID::eNEAR].w = clip[15] + clip[14];

	// Normalize each plane
	for (int i = 0; i < 6; ++i)
	{
		const glm::vec4 &plane = mPlanes[i];
		float dist = glm::sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
#ifdef _DEBUG
		if (dist == 0.0f)
		{
			assert(false);
		}
#endif
		mPlanes[i] /= dist;
	}
}

const glm::vec4& Frustrum::getNormalizedPlane(Frustrum::PlaneID index) const
{
	return mPlanes[index];
}

bool Frustrum::checkPoint(const glm::vec3 &point) const
{
	for (int i = 0; i < 6; ++i)
	{
		const glm::vec4 &plane = mPlanes[i];
		if (plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w <= 0)
			return false;
	}
	return true;
}

bool Frustrum::checkSphere(const glm::vec3 &point, float radius) const
{
	for (int i = 0; i < 6; ++i)
	{
		const glm::vec4 &plane = mPlanes[i];
		if (plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w <= -radius)
			return false;
	}
	return true;
}

bool Frustrum::checkCubeFAST(const glm::vec3 &center, const glm::vec3 &length) const
{
	// Calc min/maxes
	float xMin = center.x - length.x;
	float xMax = center.x + length.x;
	float yMin = center.y - length.y;
	float yMax = center.y + length.y;
	float zMin = center.z - length.z;
	float zMax = center.z + length.z;

#define check(_x, _y, _z) frustrum.x * _x + frustrum.y * _y + frustrum.z * _z + frustrum.w > 0 

	for (int i = 0; i < 6; ++i)
	{
		const glm::vec4 &frustrum = mPlanes[i];
		if (check(xMin, yMin, zMin))
			continue;
		if (check(xMax, yMin, zMin))
			continue;
		if (check(xMin, yMax, zMin))
			continue;
		if (check(xMax, yMax, zMin))
			continue;
		if (check(xMin, yMin, zMax))
			continue;
		if (check(xMax, yMin, zMax))
			continue;
		if (check(xMin, yMax, zMax))
			continue;
		if (check(xMax, yMax, zMax))
			continue;
		return false;
	}
	return true;

#undef check
}