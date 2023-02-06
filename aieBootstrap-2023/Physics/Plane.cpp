#include "Plane.h"
#include "Gizmos.h"
#include "glm/glm.hpp"
#include <iostream>

Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);
}

Plane::Plane(glm::vec2 _normal, float _distance, glm::vec4 _color) 
{
	m_normal = _normal;
	m_distanceToOrigin = _distance;
	m_color = _color;
	m_shapeID = ShapeType::PLANE;
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

	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colorFade = m_color;
	colorFade.a = 0;
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

	//aie::Gizmos::add2DLine(start, end, m_color);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.f, m_color, m_color, colorFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.f, start - m_normal * 10.f, m_color, colorFade, colorFade);
}

void Plane::ResetPosition()
{
}

void Plane::ResolveCollision(RigidBody* _actor, glm::vec2 _contact)
{
	glm::vec2 localContact = _contact - _actor->GetPos();

	// plane is stationary -> relative velocity is just actor velocity at contact point
	glm::vec2 velRel = _actor->GetVel() + _actor->GetAngularVel() * glm::vec2(-localContact.y, localContact.x);
	float velIntoPlane = glm::dot(velRel, m_normal);

	float e = 1;

	float r = glm::dot(localContact, glm::vec2(m_normal.y, m_normal.x));

	// this is the effective mass
	float mass0 = 1.0f / (1.0f / _actor->GetMass() + (r * r) / _actor->GetMoment());

	float j = -(1 + e) * velIntoPlane * mass0;

	glm::vec2 force = m_normal * j;

	float kePre = _actor->CalcKineticEnergy();
	_actor->ApplyForce(force, _contact - _actor->GetPos());
	float kePost = _actor->CalcKineticEnergy();

	if (kePost - kePre > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy";
}
