#pragma once

#include "PhysicsObject.h"
#include <glm/glm.hpp>

class RigidBody : public PhysicsObject
{
public:
	RigidBody();
	RigidBody(ShapeType _shapeID, glm::vec2 _pos,
		glm::vec2 _vel, float _orientation, float _mass,
		glm::vec4 _color);
	~RigidBody();

	virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);

	void ApplyForce(glm::vec2 _force);
	void ApplyForceToActor(RigidBody* _actorOther, glm::vec2 _force);

	// Getters
	glm::vec2 GetPos() { return m_pos; }
	glm::vec2 GetVel() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetOrientation() { return m_orientation; }
	glm::vec4 GetColor() { return m_color; }

	// Setters
	void SetPos(glm::vec2 _pos) { m_pos = _pos; }
	void SetVel(glm::vec2 _vel) { m_velocity = _vel; }
	void SetMass(float _mass) { m_mass = _mass; }
	void SetOrientation(float _orientation) { m_orientation = _orientation; }
	void SetColor(glm::vec4 _color) { m_color = _color; }

protected:
	glm::vec2 m_pos;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation;

	glm::vec4 m_color;

};

