#pragma once

#include "PhysicsObject.h"
#include <glm/glm.hpp>
#include <functional>
#include <list>

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

	void SetKinematic(bool _state) { m_isKinematic = _state; }
	bool IsKinematic() { return m_isKinematic; }

	void ResolveCollision(RigidBody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNorm = nullptr, float _pen = 0);
	virtual float CalcKineticEnergy();
	float CalcPotentialEnergy();
	virtual float GetEnergy();

	void CalculateSmoothedPosition(float _alpha);
	void CalculateAxes();
	glm::vec2 ToWorld(glm::vec2 _local, float _alpha);
	glm::vec2 ToWorldSmoothed(glm::vec2 _localPos);

	// Getters
	glm::vec2 GetPos() const { return m_pos; }
	glm::vec2 GetVel() { return m_vel; }
	float GetMass() { return m_isKinematic ? INT_MAX : m_mass; }

	float GetOrientation() { return m_orientation; }
	float GetAngularVel() { return m_angularVel; }
	float GetMoment() { return m_isKinematic ? INT_MAX : m_moment; }

	glm::vec4 GetColor() { return m_color; }
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }

	float GetLinearDrag() { return m_linearDrag; }
	float GetAngularDrag() { return m_angularDrag; }

	bool GetTrigger() { return m_isTrigger; }

	// Setters
	void SetPos(glm::vec2 _pos) { m_pos = _pos; }
	void SetVel(glm::vec2 _vel) { m_vel = _vel; }
	void SetMass(float _mass) { m_mass = _mass; }

	void SetOrientation(float _orientation) { m_orientation = _orientation; }
	void SetAngularVel(float _angularVel) { m_angularVel = _angularVel; }
	void SetMoment(float _moment) { m_moment = _moment; }

	void SetColor(glm::vec4 _color) { m_color = _color; }

	void SetLinearDrag(float _linearDrag) { m_linearDrag = _linearDrag; }
	void SetAngularDrag(float _angularDrag) { m_angularDrag = _angularDrag; }

	void SetTrigger(bool _isTrigger) { m_isTrigger = _isTrigger; }

	std::function<void(PhysicsObject*)> collisionCallback;

	void TriggerEnter(PhysicsObject* _actor);

	std::function<void(PhysicsObject*)> triggerEnter;
	std::function<void(PhysicsObject*)> triggerExit;

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

	float m_linearDrag;
	float m_angularDrag;

	bool m_isKinematic;

	glm::vec2 m_worldSmooth;

	bool m_isTrigger;
	std::list<PhysicsObject*> m_objectsInside;
	std::list<PhysicsObject*> m_objectsInsideThisFrame;
};

