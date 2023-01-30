#include "RigidBody.h"

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
	ApplyForce(_gravity * m_mass * _timeStep);
}

void RigidBody::ApplyForce(glm::vec2 _force)
{
	m_velocity += _force;
}

void RigidBody::ApplyForceToActor(RigidBody* _actorOther, glm::vec2 _force)
{
	ApplyForce(_force);
	_actorOther->ApplyForce(-(_force));
}
