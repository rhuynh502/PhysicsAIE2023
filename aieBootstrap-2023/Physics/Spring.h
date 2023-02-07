#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"
#include <glm/glm.hpp>

class Spring : public PhysicsObject
{
public:
	virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
	virtual void Draw(float _alpha);

	// Getters
	glm::vec2 GetContact1(float _alpha) { return m_body1 ? m_body1->ToWorld(m_contact1, _alpha) : m_contact1; }
	glm::vec2 GetContact2(float _alpha) { return m_body2 ? m_body2->ToWorld(m_contact2, _alpha) : m_contact2; }

protected:
	Spring(RigidBody* _body1, RigidBody* _body2,
		float _springCoefficient, float _damping, float _restLength = 0,
		glm::vec2 _contact1 = glm::vec2(0), glm::vec2 _contact2 = glm::vec2(0));
	~Spring();

	RigidBody* m_body1;
	RigidBody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	glm::vec4 m_color;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;
};

