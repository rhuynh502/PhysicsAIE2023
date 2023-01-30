#pragma once
#include <glm/vec2.hpp>

class PhysicsObject
{
protected:
	PhysicsObject();
	~PhysicsObject();

public:
	virtual void FixedUpdate(glm::vec2 _gravity,
		float _timeStep) = 0;
	virtual void Draw(float _alpha) = 0;
	virtual void ResetPosition() {};


private:
};

