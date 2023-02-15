#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "PhysicsObject.h"
#include "RigidBody.h"

class PhysicsScene
{
	static glm::vec2 m_gravity;
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* _actor);
	void RemoveActor(PhysicsObject* _actor);
	void AddToRemove(PhysicsObject* _actor);
	
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

	static void ApplyContactForces(RigidBody* _body1, RigidBody* _body2, glm::vec2 _norm, float _pen);

	static bool Plane2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Plane2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Plane2Box(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Circle2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Circle2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Circle2Box(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Box2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Box2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);
	static bool Box2Box(PhysicsObject* _obj1, PhysicsObject* _obj2);

protected:
	float m_timeStep;

	std::vector<PhysicsObject*> m_actors;
	std::vector<PhysicsObject*> m_actorsToRemove;
};

