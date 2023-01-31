#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 _normal, float _distance, glm::vec4 _color);
	~Plane();

	virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
	virtual void Draw(float _alpha);
	virtual void ResetPosition();

	// Getters
	glm::vec2 GetNormal() { return m_normal; }
	float GetDistance() { return m_distanceToOrigin; }

	// Setters
	void SetNormal(glm::vec2 _norm) { m_normal = _norm; }
	void SetDistance(float _distance) { m_distanceToOrigin = _distance; }

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm::vec4 m_color;
};

