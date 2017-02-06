//-----------------------------------------------------------------------------
// frustrum.cpp
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
//
// Reference Note: 
// Frustrum math implementation taken from this artifact from the interwebs:
// http://www.crownandcutlass.com/features/technicaldetails/frustum.html
// This page and its contents are Copyright 2000 by Mark Morley
// Unless otherwise noted, you may use any and all code examples provided herein 
// in any way you want.
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
	glm::mat4 vp = view * proj;
	float *clip = &vp[0][0];

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
		glm::vec4 &plane = mPlanes[i];
		float dist = glm::sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
#ifdef _DEBUG
		if (dist == 0.0f)
		{
			assert(false);
		}
#endif
		plane *= 1.0f / dist;
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

bool Frustrum::checkCubeFAST(const glm::vec3 &center, float length) const
{
	// Calc min/maxes
	float xMin = center.x - length;
	float xMax = center.x + length;
	float yMin = center.y - length;
	float yMax = center.y + length;
	float zMin = center.z - length;
	float zMax = center.z + length;

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