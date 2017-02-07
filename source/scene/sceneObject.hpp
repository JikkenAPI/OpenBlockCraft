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

#ifndef _SCENE_SCENEOBJECT_HPP_
#define _SCENE_SCENEOBJECT_HPP_

#include <glm/glm.hpp>

class SceneObject
{
public:
	SceneObject();
	virtual ~SceneObject();

	void setPosition(const glm::vec3 &pos) { mPosition = pos; }
	glm::vec3 getPosition() const { return mPosition;  }

	virtual void update(const double &dt) {}

protected:
	glm::vec3 mPosition;
};

#endif