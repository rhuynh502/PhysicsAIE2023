#pragma once
#include <glm/glm.hpp>

enum ShapeType
{
	PLANE = 0,
	CIRCLE,
	BOX
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType _shapeID) : m_shapeID(_shapeID) {}

	PhysicsObject();
	~PhysicsObject();

public:
	virtual void FixedUpdate(glm::vec2 _gravity,
		float _timeStep) = 0;
	virtual void Draw(float _alpha) = 0;
	virtual void ResetPosition() {};

protected:
	ShapeType m_shapeID;
};

