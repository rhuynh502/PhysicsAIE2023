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

	void ApplyForce(glm::vec2 _force, glm::vec2 _pos);
	//void ApplyForceToActor(RigidBody* _actorOther, glm::vec2 _force, glm::vec2 _pos);

	void ResolveCollision(RigidBody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNorm = nullptr);
	virtual float CalcKineticEnergy();
	float CalcPotentialEnergy();
	virtual float GetEnergy();

	void CalculateSmoothedPosition(float _alpha);
	void CalculateAxes();

	// Getters
	glm::vec2 GetPos() { return m_pos; }
	glm::vec2 GetVel() { return m_vel; }
	float GetMass() { return m_mass; }
	float GetOrientation() { return m_orientation; }
	float GetAngularVel() { return m_angularVel; }
	float GetMoment() { return m_moment; }
	glm::vec4 GetColor() { return m_color; }
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }

	// Setters
	void SetPos(glm::vec2 _pos) { m_pos = _pos; }
	void SetVel(glm::vec2 _vel) { m_vel = _vel; }
	void SetMass(float _mass) { m_mass = _mass; }
	void SetOrientation(float _orientation) { m_orientation = _orientation; }
	void SetAngularVel(float _angularVel) { m_angularVel = _angularVel; }
	void SetMoment(float _moment) { m_moment = _moment; }
	void SetColor(glm::vec4 _color) { m_color = _color; }

protected:
	glm::vec2 m_pos;
	glm::vec2 m_vel;
	glm::vec2 m_lastPos;
	float m_mass;
	float m_orientation;
	float m_angularVel;
	float m_moment;

	float m_lastOrientation;

	glm::vec4 m_color;

	glm::vec2 m_smoothedPosition;
	glm::vec2 m_smoothedLocalX;
	glm::vec2 m_smoothedLocalY;

	glm::vec2 m_localX;
	glm::vec2 m_localY;

};

