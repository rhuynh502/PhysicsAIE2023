#include "Circle.h"
#include "Gizmos.h"
#include "Demos.h"

Circle::Circle(glm::vec2 _pos, glm::vec2 _vel, float _mass, float _radius, glm::vec4 _color) :
	RigidBody(CIRCLE, _pos, _vel, 0.f, _mass, _color)
{
	m_pos = _pos;
	m_velocity = _vel;
	m_mass = _mass;
	m_radius = _radius;
	m_color = _color;
}

void Circle::Draw(float _alpha)
{
	aie::Gizmos::add2DCircle(m_pos, m_radius, 50, m_color);

	float angle = m_orientation * (PI / 180.f);
	aie::Gizmos::add2DLine(m_pos, glm::vec2((m_pos.x + m_radius * glm::cos(angle)), (m_pos.y + m_radius * glm::sin(angle))), glm::vec4(1, 0, 0, 1));
}
