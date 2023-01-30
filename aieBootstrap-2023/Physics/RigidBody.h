#pragma once

#include "PhysicsObject.h"
#include <glm/glm.hpp>

class RigidBody : public PhysicsObject
{
public:
	RigidBody();
	~RigidBody();

	virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);

	void ApplyForce(glm::vec2 _force);
	void ApplyForceToActor(RigidBody* _actorOther, glm::vec2 _force);

	// Getters
	glm::vec2 GetPos() { return m_pos; }
	glm::vec2 GetVel() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetOrientation() { return m_orientation; }

	// Setters
	void SetPos(glm::vec2 _pos) { m_pos = _pos; }
	void SetVel(glm::vec2 _vel) { m_velocity = _vel; }
	void SetMass(float _mass) { m_mass = _mass; }
	void SetOrientation(float _orientation) { m_orientation = _orientation; }

protected:
	glm::vec2 m_pos;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation;

	glm::vec4 m_color;

};

