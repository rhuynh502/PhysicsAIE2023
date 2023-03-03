#include "Circle.h"
#include "Gizmos.h"
#include "Demos.h"

Circle::Circle(glm::vec2 _pos, glm::vec2 _vel, float _mass, float _radius, glm::vec4 _color) :
	RigidBody(CIRCLE, _pos, _vel, 0.f, _mass, _color)
{
	m_radius = _radius;

	m_angularVel = 0;
	
	m_moment = 0.5f * m_mass * m_radius * m_radius;

	m_isKinematic = false;
}

Circle::~Circle()
{
}

void Circle::Draw(float _alpha)
{
	CalculateSmoothedPosition(_alpha);

	aie::Gizmos::add2DCircle(m_pos, m_radius, 6, m_color);

	if(!IsKinematic())
		aie::Gizmos::add2DLine(m_pos, m_pos + m_localX * m_radius,
			glm::vec4(1, 1, 1, 1));

}

bool Circle::IsInside(glm::vec2 _point)
{
	return glm::distance(m_pos, _point) <= m_radius;
}
