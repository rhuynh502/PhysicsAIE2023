#include "RigidBody.h"

void RigidBody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
	m_pos += m_velocity * _timeStep;
	ApplyForce(_gravity * m_mass * _timeStep);
}
