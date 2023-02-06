#pragma once
#include "RigidBody.h"
#include <glm/glm.hpp>

class Box : public RigidBody
{
public:
	Box(glm::vec2 _pos, glm::vec2 _vel,
		float _mass, glm::vec2 _extents,
		glm::vec4 _color);
	Box(glm::vec2 _pos, glm::vec2 _vel,
		float _mass, float _width, float _height,
		glm::vec4 _color);
	~Box();

	virtual void Draw(float _alpha);

	// Getters
	glm::vec2 GetExtents() { return m_extents; }
	float GetWidth() { return m_width; }
	float GetHeight() { return m_height; }

protected:
	glm::vec2 m_extents;
	float m_width;
	float m_height;
};
