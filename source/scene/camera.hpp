//-----------------------------------------------------------------------------
// camera.hpp
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
// Note: Some of the camera code was borrowed from learnopengl.com, which is
// licensed under CC0 Universal 1.0
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

	// angles
	float mYaw;
	float mPitch;

	void _updateCamera();
};

#endif