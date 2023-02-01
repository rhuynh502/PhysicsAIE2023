#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "PhysicsObject.h"

class PhysicsObject;

class PhysicsScene
{
	static glm::vec2 m_gravity;
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* _actor);
	void RemoveActor(PhysicsObject* _actor);
	
	void Update(float _dt);
	void Draw();
	void DebugScene();

	//Getters
	static glm::vec2 GetGravity() { return m_gravity; }
	float GetTimeStep() { return m_timeStep; }

	//Setters
	void SetGravity(const glm::vec2 _gravity)
		{ m_gravity = _gravity; }
	void SetTimeStep(const float _timeStep)
		{ m_timeStep = _timeStep; }

	void CheckForCollision();
	float GetTotalEnergy();

	static bool Plane2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Plane2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Circle2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Circle2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);

private:
	float m_timeStep;

	std::vector<PhysicsObject*> m_actors;
};

