#include "Circle.h"
#include "Gizmos.h"

Circle::Circle(glm::vec2 _pos, glm::vec2 _vel, float _mass, float _radius, glm::vec4 _color)
{
	m_pos = _pos;
	m_velocity = _vel;
	m_mass = _mass;
	m_radius = _radius;
	m_color = _color;
}

void Circle::Draw(float _alpha)
{
	aie::Gizmos::add2DCircle(m_pos, m_radius, 10, m_color);
	
}
