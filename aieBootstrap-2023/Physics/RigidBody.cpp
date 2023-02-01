#include "RigidBody.h"
#include <iostream>
#include "glm/glm.hpp"
#include "PhysicsScene.h"

RigidBody::RigidBody()
{
}

RigidBody::RigidBody(ShapeType _shapeID, glm::vec2 _pos, glm::vec2 _vel, float _orientation, float _mass, glm::vec4 _color = glm::vec4(0, 0, 0, 0))
{
	m_shapeID = _shapeID;
	m_pos = _pos;
	m_velocity = _vel;
	m_orientation = _orientation;
	m_mass = _mass;
	m_color = _color;
}

RigidBody::~RigidBody()
{
}

void RigidBody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
	m_pos += m_velocity * _timeStep;
	ApplyForce(PhysicsScene::GetGravity() * m_mass * _timeStep);
}

void RigidBody::ApplyForce(glm::vec2 _force)
{
	m_velocity += _force / m_mass;
}

void RigidBody::ApplyForceToActor(RigidBody* _actorOther, glm::vec2 _force)
{
	ApplyForce(-_force);
	_actorOther->ApplyForce(_force);
}

void RigidBody::ResolveCollision(RigidBody* _actor2)
{
	glm::vec2 normal = glm::normalize(_actor2->GetPos() - m_pos);
	glm::vec2 relVel = _actor2->GetVel() - m_velocity;

	if (glm::dot(normal, relVel) >= 0)
		return;

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relVel), normal) / ((1 / GetMass()) + (1 / _actor2->GetMass()));

	glm::vec2 force = normal * j;

	float kePre = CalcKineticEnergy() + _actor2->CalcKineticEnergy();

	ApplyForceToActor(_actor2, force);

	float kePost = CalcKineticEnergy() + _actor2->CalcKineticEnergy();

	if (kePost - kePre > kePost * 0.01f)
	{
		std::cout << "Kinetic Energy discrepancy greater than 1% detected!";
	}
}

float RigidBody::CalcKineticEnergy()
{
	glm::vec2 currVel = GetVel();
	float vel = glm::sqrt(currVel.x * currVel.x + currVel.y * currVel.y);
	return 0.5f * GetMass() * vel * vel;
}

float RigidBody::CalcPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPos());
}

float RigidBody::GetEnergy()
{
	return CalcKineticEnergy() + CalcPotentialEnergy();
}
