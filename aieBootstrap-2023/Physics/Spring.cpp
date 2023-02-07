#include "Spring.h"
#include "Gizmos.h"
#include <glm/glm.hpp>

Spring::Spring(RigidBody* _body1, RigidBody* _body2,
	float _springCoefficient, float _damping, 
	glm::vec4 _color,
	float _restLength,
	glm::vec2 _contact1, glm::vec2 _contact2) :
	PhysicsObject(JOINT)
	
{
	m_body1 = _body1;
	m_body2 = _body2;

	m_springCoefficient = _springCoefficient;
	m_damping = _damping;
	
	m_contact1 = _contact1;
	m_contact2 = _contact2;

	if (_restLength == 0)
	{
		if (m_body1) m_body1->CalculateAxes();
		if (m_body2) m_body2->CalculateAxes();
		m_restLength = glm::distance(GetContact1(1), GetContact2(1));
	}
	else
		m_restLength = _restLength;
	m_color = _color;
}

Spring::~Spring()
{
}

void Spring::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
	m_body1->CalculateSmoothedPosition(1);
	m_body2->CalculateSmoothedPosition(1);

	glm::vec2 p1 = GetContact1(1);
	glm::vec2 p2 = GetContact2(1);

	float length = glm::distance(p1, p2);
	glm::vec2 direction = glm::normalize(p2 - p1);

	glm::vec2 relVel = m_body2->GetVel() - m_body1->GetVel();

	glm::vec2 force = direction * m_springCoefficient * (m_restLength - length) - m_damping * relVel;

	m_body1->ApplyForce(-force * _timeStep, p1 - m_body1->GetPos());
	m_body2->ApplyForce(force * _timeStep, p2 - m_body2->GetPos());
}

void Spring::Draw(float _alpha)
{

	m_body1->ToWorldSmoothed(m_body1->GetPos());
	m_body2->ToWorldSmoothed(m_body2->GetPos());

	aie::Gizmos::add2DLine(GetContact1(_alpha), GetContact2(_alpha), glm::vec4(0.5f, 1, 0.3f, 1));
}

