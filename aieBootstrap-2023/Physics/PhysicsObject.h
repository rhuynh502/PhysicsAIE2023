#pragma once
#include <glm/glm.hpp>

enum ShapeType
{
	PLANE = 0,
	CIRCLE,
	BOX,
	SHAPE_COUNT,
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

	virtual float CalcKineticEnergy() { return 0; };
	virtual float GetEnergy() { return 0; }

	// Getters
	int GetShapeID() { return m_shapeID; }
	float GetElasticity() { return m_elasticity; }

	// Setters
	void SetElasticity(float _elasticity) { m_elasticity = _elasticity; }

protected:
	ShapeType m_shapeID;
	float m_elasticity;
};

