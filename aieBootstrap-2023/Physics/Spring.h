#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"
#include <glm/glm.hpp>

class Spring : public PhysicsObject
{
public:
	Spring(RigidBody* _body1, RigidBody* _body2,
		float _springCoefficient, float _damping, 
		glm::vec4 _color,
		float _restLength = 0,
		glm::vec2 _contact1 = glm::vec2(0), glm::vec2 _contact2 = glm::vec2(0));
	~Spring();
	virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
	virtual void Draw(float _alpha);

	// Getters
	glm::vec2 GetContact1(float _alpha) { return m_body1 ? m_body1->ToWorldSmoothed(m_contact1) : m_contact1; }
	glm::vec2 GetContact2(float _alpha) { return m_body2 ? m_body2->ToWorldSmoothed(m_contact2) : m_contact2; }

protected:

	RigidBody* m_body1;
	RigidBody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	glm::vec4 m_color;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;
};

