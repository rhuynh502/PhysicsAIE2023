#include "Plane.h"
#include "Gizmos.h"
#include "glm/glm.hpp"
#include <iostream>
#include "PhysicsScene.h"

Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);
	m_elasticity = 1;
	m_staticFriction = 0;
	m_kinecticFriction = 0;
}

Plane::Plane(glm::vec2 _normal, float _distance, glm::vec4 _color) :
	PhysicsObject(ShapeType::PLANE)
{
	m_normal = _normal;
	m_distanceToOrigin = _distance;
	m_color = _color;
	m_elasticity = 1;
	m_staticFriction = 0;
	m_kinecticFriction = 0;
}

Plane::Plane(glm::vec2 _normal, float _distance) : 
	PhysicsObject(ShapeType::PLANE)
{
	m_normal = _normal;
	m_distanceToOrigin = _distance;
	m_elasticity = 1;
	m_color = glm::vec4(1, 1, 1, 1);
	m_staticFriction = 0;
	m_kinecticFriction = 0;
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
	// the position at which we'll apply the force relative to the object's COM
	glm::vec2 localContact = _contact - _actor->GetPos();

	// the plane isn't moving, so the relative velocity is just actor2's velocity at the contact point
	glm::vec2 vRel = _actor->GetVel() + _actor->GetAngularVel() * glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, m_normal);

	// perfectly elasticity collisions for now
	float e = (GetElasticity() + _actor->GetElasticity()) / 2.0f;

	// this is the perpendicular distance we apply the force at relative to the COM, so Torque = F*r
	float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

	// work out the "effective mass" - this is a combination of moment of
	// inertia and mass, and tells us how much the contact point velocity 
	// will change with the force we're applying
	float mass0 = 1.0f / (1.0f / _actor->GetMass() + (r * r) / _actor->GetMoment());

	float j = -(1 + e) * velocityIntoPlane * mass0;

	glm::vec2 fricForce = (GetStaticFriction() + _actor->GetKineticFriction()) / 2
		/ (1 / _actor->GetMass())
		* PhysicsScene::GetGravity()
		* glm::cos(glm::atan(m_normal.y / m_normal.x));

	glm::vec2 linearAccel = (_actor->GetMass() * PhysicsScene::GetGravity() * glm::sin(m_normal.y / m_normal.x) * _actor->GetMoment() / (_actor->GetMass() * localContact * localContact + _actor->GetMoment()));

	glm::vec2 force = m_normal * j - fricForce;

	float kePre = _actor->CalcKineticEnergy();

	_actor->ApplyForce(force, _contact - _actor->GetPos());

	float pen = glm::dot(_contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(_actor, nullptr, m_normal, pen);

	float kePost = _actor->CalcKineticEnergy();

	float deltaKE = kePost - kePre;
	if (deltaKE > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}
