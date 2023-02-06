#include "Box.h"
#include "Gizmos.h"

Box::Box(glm::vec2 _pos, glm::vec2 _vel,
	float _mass, glm::vec2 _extents,
	glm::vec4 _color) :
	RigidBody(BOX, _pos, _vel, 0.f, _mass, _color)
{
	m_pos = _pos;
	m_vel = _vel;
	m_mass = _mass;
	m_extents = _extents;
	m_color = _color;
	m_angularVel = 0;

	m_width = _extents.x * 2;
	m_height = _extents.y * 2;

	m_moment = 1.0f / 12.0f * m_mass * m_width * m_height;
}

Box::Box(glm::vec2 _pos, glm::vec2 _vel, 
	float _mass, float _width, float _height,
	glm::vec4 _color) :
	RigidBody(BOX, _pos, _vel, 0.f, _mass, _color)
{
	m_pos = _pos;
	m_vel = _vel;
	m_mass = _mass;
	m_width = _width;
	m_height = _height;
	m_color = _color;

	m_extents = glm::vec2(_width / 2, _height / 2);

	m_moment = 1.0f / 12.0f * m_mass * m_width * m_height;
}

Box::~Box()
{
}

void Box::Draw(float _alpha)
{
	CalculateSmoothedPosition(_alpha);

	glm::vec2 p1 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
		- m_smoothedLocalY * m_extents.y;
	glm::vec2 p2 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
		- m_smoothedLocalY * m_extents.y;
	glm::vec2 p3 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
		+ m_smoothedLocalY * m_extents.y;
	glm::vec2 p4 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
		+ m_smoothedLocalY * m_extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, m_color);
	aie::Gizmos::add2DTri(p1, p4, p3, m_color);

}
