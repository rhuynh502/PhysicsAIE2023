#pragma once

#include "RigidBody.h"
#include <glm/glm.hpp>

class Circle : public RigidBody
{
public:
	Circle(glm::vec2 _pos, glm::vec2 _vel, 
		float _mass, float _radius,
		glm::vec4 _color);
	~Circle();

	virtual void Draw(float _alpha);

	// Getters
	float GetRadius() { return m_radius; }

	// Setters
	void SetRadius(float _radius) { m_radius = _radius; }

protected:
	float m_radius;

};

