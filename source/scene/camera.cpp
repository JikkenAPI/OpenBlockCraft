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

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "scene/camera.hpp"

const float MOUSE_SENSITIVITY = 0.1f;

const float CAMERA_SPEED = 0.01f;

Camera::Camera()
{
	mFrontVector = glm::vec3(0.0f, 0.0f, -1.0f);
	mWorldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	mRightVector = glm::vec3(0.0f, 0.0f, 0.0f);
	mUpVector = glm::vec3(0.0f, 0.0f, 0.0f);

	mLastMousePos = glm::vec2(0.0f, 0.0f);
	mFirstMouseMove = true;

	mYaw = -90.0f;
	mPitch = 0.0f;

	mMoveAxis.horizontal = 0.0f;
	mMoveAxis.vertical = 0.0f;
}

Camera::~Camera()
{

}

void Camera::onKeyPressEvent(const KeyPressEventData &data)
{
	float val = (data.keyState == Input::KeyState::ePRESSED) ? 1.0f : -1.0f;
	switch (data.key)
	{
		case Input::Key::eW:
		case Input::Key::eUP:
			mMoveAxis.horizontal += val;
			break;
		case Input::Key::eS:
		case Input::Key::eDOWN:
			mMoveAxis.horizontal -= val;
			break;
		case Input::Key::eD:
		case Input::Key::eRIGHT:
			mMoveAxis.vertical += val;
			break;
		case Input::Key::eA:
		case Input::Key::eLEFT:
			mMoveAxis.vertical -= val;
			break;
	}
}

void Camera::onMouseMoveEvent(const MousePositionData &data)
{
	// record first movement. Second movent will move it.
	if (mFirstMouseMove)
	{
		mLastMousePos = glm::vec2(data.x, data.y);
		mFirstMouseMove = false;
		return;
	}

	mYaw += (static_cast<float>(data.x) - mLastMousePos.x) * MOUSE_SENSITIVITY;
	mPitch += (mLastMousePos.y - static_cast<float>(data.y)) * MOUSE_SENSITIVITY; // Reversed y

	mLastMousePos = glm::vec2(data.x, data.y);

	// pitch clamping
	mPitch = glm::clamp(mPitch, -90.0f, 90.0f);
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mPosition, mPosition + mFrontVector, mUpVector);
}

void Camera::getYawPitch(float &yaw, float &pitch) const
{
	yaw = mYaw;
	pitch = mPitch;
}

void Camera::update(const double &dt)
{
	float yaw = glm::radians(mYaw);
	float pitch = glm::radians(mPitch);

	// Calculate front vector.
	glm::vec3 front(
		std::cos(yaw) * std::cos(pitch),
		std::sin(pitch),
		std::sin(yaw) * std::cos(pitch)
	);
	mFrontVector = glm::normalize(front);

	// Update right and up vectors
	mRightVector = glm::normalize(glm::cross(mFrontVector, mWorldUpVector));
	mUpVector = glm::normalize(glm::cross(mRightVector, mFrontVector));

	// Update position
	const float SPEED = static_cast<float>(dt) * CAMERA_SPEED;
	mPosition += mFrontVector * SPEED * mMoveAxis.horizontal;
	mPosition += mRightVector * SPEED * mMoveAxis.vertical;
}
