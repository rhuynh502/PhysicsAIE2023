#pragma once
#include "Circle.h"

class PachinkoBall : public Circle
{
public:
	PachinkoBall(glm::vec2 _pos, glm::vec2 _vel,
		float _mass, float _radius,
		glm::vec4 _color,
		float _points);
	~PachinkoBall();

	float GetPoints() { return m_points; }

protected:
	float m_points;
};

