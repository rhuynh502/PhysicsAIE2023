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
	m_vel = _vel;
	m_orientation = _orientation;
	m_mass = _mass;
	m_color = _color;
	m_angularVel = 0;
}

RigidBody::~RigidBody()
{
}

void RigidBody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
	m_lastPos = m_pos;
	m_pos += m_vel * _timeStep;
	ApplyForce(PhysicsScene::GetGravity() * m_mass * _timeStep, m_pos);

	m_lastOrientation = m_orientation;
	m_orientation += m_angularVel * _timeStep;
}

void RigidBody::ApplyForce(glm::vec2 _force, glm::vec2 _pos)
{
	m_vel += _force / GetMass();
	m_angularVel += (_force.y * _pos.x - _force.x * _pos.y) / GetMoment();
}

//void RigidBody::ApplyForceToActor(RigidBody* _actorOther, glm::vec2 _force)
//{
//	ApplyForce(-_force);
//	_actorOther->ApplyForce(_force);
//}

void RigidBody::ResolveCollision(RigidBody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNorm)
{
	// find vec normal or use provided one
	glm::vec2 normal = glm::normalize(_collisionNorm ? *_collisionNorm : _actor2->GetPos() - m_pos);
	glm::vec2 perp(normal.y, -normal.x);
	
	float r1 = glm::dot(_contact - m_pos, -perp);
	float r2 = glm::dot(_contact - _actor2->m_pos, perp);

	float v1 = glm::dot(m_vel, normal) - r1 * m_angularVel;
	float v2 = glm::dot(_actor2->m_vel, normal) + r2 * _actor2->m_angularVel;

	if (v1 > v2) // moving closer
	{
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / _actor2->m_mass + (r1 * r1) / _actor2->m_moment);

		float elasticity = 1;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		ApplyForce(-force, _contact - m_pos);
		_actor2->ApplyForce(force, _contact - _actor2->m_pos);
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

void RigidBody::CalculateSmoothedPosition(float _alpha)
{
	m_smoothedPosition = _alpha * m_pos + (1 - _alpha) * m_lastPos;

	float smoothedOrientation = _alpha * m_orientation + (1 - _alpha) * m_lastOrientation;

	float sin = sinf(smoothedOrientation);
	float cos = cosf(smoothedOrientation);
	m_smoothedLocalX = glm::vec2(cos, sin);
	m_smoothedLocalY = glm::vec2(-sin, cos);
}
