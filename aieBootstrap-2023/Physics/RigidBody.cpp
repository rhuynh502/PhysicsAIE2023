#include "RigidBody.h"
#include <iostream>
#include "glm/glm.hpp"
#include "PhysicsScene.h"

#define MIN_LINEAR_THRESHOLD 0.09970000001f
#define MIN_ANGULAR_THRESHOLD 0.03f

RigidBody::RigidBody()
{
	m_isTrigger = false;
	m_isKinematic = false;
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
	m_linearDrag = 0.3f;
	m_angularDrag = 0.4f;
	m_elasticity = 1.f;
	m_isKinematic = false;
	m_isTrigger = false;
}

RigidBody::~RigidBody()
{
}

void RigidBody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
	CalculateAxes();

	if (m_isTrigger)
	{
		for (auto it = m_objectsInside.begin(); it != m_objectsInside.end(); it++)
		{
			if (std::find(m_objectsInsideThisFrame.begin(), m_objectsInsideThisFrame.end(), *it) == m_objectsInsideThisFrame.end())
			{
				if (triggerExit)
					triggerExit(*it);
				it = m_objectsInside.erase(it);
				if (it == m_objectsInside.end())
					break;
			}
		}
	}

	m_objectsInsideThisFrame.clear();

	if (m_isKinematic)
	{
		m_vel = glm::vec2(0);
		m_angularVel = 0;
		return;
	}

	if (glm::length(m_vel) < MIN_LINEAR_THRESHOLD)
	{
		m_vel = glm::vec2(0, 0);
	}

	if (glm::length(m_angularVel) < MIN_ANGULAR_THRESHOLD)
	{
		m_angularVel = 0;
	}

	m_lastPos = m_pos;
	m_lastOrientation = m_orientation;

	m_pos += m_vel * _timeStep;

	ApplyForce(_gravity * GetMass() * _timeStep, glm::vec2(0));

	m_orientation += m_angularVel * _timeStep;

	m_vel -= m_vel * m_linearDrag * _timeStep;
	m_angularVel -= m_angularVel * m_angularDrag * _timeStep;

}

void RigidBody::ApplyForce(glm::vec2 _force, glm::vec2 _pos)
{
	m_vel += _force / GetMass();

	float totForce = _force.y * _pos.x - _force.x * _pos.y;

	m_angularVel += totForce / GetMoment();
}

//void RigidBody::ApplyForceToActor(RigidBody* _actorOther, glm::vec2 _force)
//{
//	ApplyForce(-_force);
//	_actorOther->ApplyForce(_force);
//}

void RigidBody::ResolveCollision(RigidBody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNorm, float _pen)
{
	m_objectsInsideThisFrame.push_back(_actor2);
	_actor2->m_objectsInsideThisFrame.push_back(this);

	// find vec normal or use provided one
	glm::vec2 normal = glm::normalize(_collisionNorm ? *_collisionNorm : _actor2->GetPos() - m_pos);
	glm::vec2 relVel = _actor2->GetVel() - m_vel;
	glm::vec2 perp(normal.y, -normal.x);
	
	float r1 = glm::dot(_contact - m_pos, -perp);
	float r2 = glm::dot(_contact - _actor2->m_pos, perp);

	float v1 = glm::dot(m_vel, normal) - r1 * GetAngularVel();
	float v2 = glm::dot(_actor2->m_vel, normal) + r2 * _actor2->GetAngularVel();

	if (v1 > v2) // moving closer
	{
		float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / GetMoment());
		float mass2 = 1.0f / (1.0f / _actor2->GetMass() + (r2 * r2) / _actor2->GetMoment());

		float elasticity = (GetElasticity() + _actor2->GetElasticity()) / 2.0f;

		float j = glm::dot(-(1 + elasticity) * (relVel), normal) /
			glm::dot(normal, normal * ((1 / GetMass()) + (1 / _actor2->GetMass())));

		//glm::vec2 force = normal * j;
		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		float kePre = CalcKineticEnergy() + _actor2->CalcKineticEnergy();

		if (!m_isTrigger && !_actor2->m_isTrigger)
		{
			glm::vec2 gravVec = PhysicsScene::GetGravity();
			float gravity = glm::sqrt(gravVec.x * gravVec.x + gravVec.y * gravVec.y);

			ApplyForce(-force, _contact - m_pos);
			_actor2->ApplyForce(force, _contact - _actor2->m_pos);
			

			if (collisionCallback != nullptr)
				collisionCallback(_actor2);
			if (_actor2->collisionCallback)
				_actor2->collisionCallback(this);
		}
		else
		{
			TriggerEnter(_actor2);
			_actor2->TriggerEnter(this);
		}

		float kePost = CalcKineticEnergy() + _actor2->CalcKineticEnergy();
		if (kePost - kePre > kePost * 0.01f)
			std::cout << "kinetic energy discrepancy\n";

		if (_pen > 0)
			PhysicsScene::ApplyContactForces(this, _actor2, normal, _pen);
	}
}

float RigidBody::CalcKineticEnergy()
{
	glm::vec2 currVel = GetVel();
	float vel = glm::sqrt(currVel.x * currVel.x + currVel.y * currVel.y);
	return 0.5f * (GetMass() * glm::dot(m_vel, m_vel) + m_moment * m_angularVel * m_angularVel);
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

void RigidBody::CalculateAxes()
{
	float sin = sinf(m_orientation);
	float cos = cosf(m_orientation);
	m_localX = glm::vec2(cos, sin);
	m_localY = glm::vec2(-sin, cos);
}

glm::vec2 RigidBody::ToWorld(glm::vec2 _local, float _alpha)
{
	return m_pos + m_localX * _local.x + m_localY * _local.y;
}

glm::vec2 RigidBody::ToWorldSmoothed(glm::vec2 _localPos)
{
	return m_smoothedPosition + m_smoothedLocalX * _localPos.x + m_smoothedLocalY * _localPos.y;
}

void RigidBody::TriggerEnter(PhysicsObject* _actor)
{
	if (m_isTrigger && std::find(m_objectsInside.begin(), m_objectsInside.end(), _actor) == m_objectsInside.end())
	{
		m_objectsInside.push_back(_actor);
		if (triggerEnter != nullptr)
			triggerEnter(_actor);
	}
}
