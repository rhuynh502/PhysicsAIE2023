#include "Plane.h"
#include "Gizmos.h"

Plane::Plane(glm::vec2 _normal, float _distance, glm::vec4 _color)
{
	m_normal = _normal;
	m_distanceToOrigin = _distance;
	m_color = _color;
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
}

void Plane::Draw(float _alpha)
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;

	glm::vec2 parallel(m_normal.y, m_normal.x);
	glm::vec4 colorFade = m_color;
	colorFade.a = 0;
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

	aie::Gizmos::add2DLine(start, end, m_color);
	/*aie::Gizmos::add2DTri(start, end, start - m_normal * 10.f, m_color, m_color, colorFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.f, start - m_normal * 10.f, m_color, colorFade, colorFade);*/
}

void Plane::ResetPosition()
{
}
