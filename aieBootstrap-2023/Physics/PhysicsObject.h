#pragma once
#include <glm/glm.hpp>

enum ShapeType
{
	JOINT = -1,
	PLANE = 0,
	CIRCLE,
	BOX,
};
	
const int SHAPE_COUNT = 3;

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
	float GetStaticFriction() { return m_staticFriction; }
	float GetKineticFriction() { return m_kinecticFriction; }

	// Setters
	void SetElasticity(float _elasticity) { m_elasticity = _elasticity; }
	void SetStaticFriction(float _staticFriction) { m_staticFriction = _staticFriction; }
	void SetKineticFriction(float _kinecticFriction) { m_kinecticFriction = _kinecticFriction; }

protected:
	ShapeType m_shapeID;
	float m_elasticity;

	float m_staticFriction;
	float m_kinecticFriction;
};

