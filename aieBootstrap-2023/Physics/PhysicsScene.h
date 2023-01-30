#pragma once
#include <glm/vec2.hpp>
#include <vector>

#include "PhysicsObject.h"

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* _actor);
	void RemoveActor(PhysicsObject* _actor);
	
	void Update(float _dt);
	void Draw();

	//Getters
	glm::vec2 GetGravity() { return m_gravity; }
	float GetTimeStep() { return m_timeStep; }

	//Setters
	void SetGravity(const glm::vec2 _gravity)
		{ m_gravity = _gravity; }
	void SetTimeStep(const float _timeStep)
		{ m_timeStep = _timeStep; }

	static bool Circle2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);

private:
	glm::vec2 m_gravity;
	float m_timeStep;

	std::vector<PhysicsObject*> m_actors;
};

