#include "PachinkoBall.h"

PachinkoBall::PachinkoBall(glm::vec2 _pos, glm::vec2 _vel,
	float _mass, float _radius,
	glm::vec4 _color, float _points) : Circle(_pos, _vel, _mass, _radius, _color)
{
	m_points = _points;
}

PachinkoBall::~PachinkoBall()
{
}
