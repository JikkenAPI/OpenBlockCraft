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

#ifndef _SCENE_CAMERA_HPP_
#define _SCENE_CAMERA_HPP_

#include "scene/sceneObject.hpp"
#include "platform/input/iInputListener.hpp"

class Camera : public SceneObject, public IInputListener
{
public:
	Camera();
	virtual ~Camera();

	virtual void onKeyPressEvent(const KeyPressEventData &data) override;

	virtual void onMouseMoveEvent(const MousePositionData &data) override;

	virtual void update(const double &dt) override;

	glm::mat4 getViewMatrix() const;

	void getYawPitch(float &yaw, float &pitch) const;

protected:
	glm::vec3 mFrontVector;
	glm::vec3 mUpVector;
	glm::vec3 mRightVector;
	glm::vec3 mWorldUpVector;

	/// Used so we can calculate mouse delta for movement.
	glm::vec2 mLastMousePos;
	bool mFirstMouseMove;

	struct 
	{
		float horizontal;
		float vertical;
	} 
	mMoveAxis;

	// angles
	float mYaw;
	float mPitch;
};

#endif