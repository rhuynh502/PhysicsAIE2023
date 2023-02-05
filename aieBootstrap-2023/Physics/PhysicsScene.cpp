#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"
#include "Demos.h"
#include <iostream>

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

glm::vec2 PhysicsScene::m_gravity(0, 0);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,
};

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01;
	m_gravity = glm::vec2(0, 0);
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete &pActor;
	}
}

void PhysicsScene::AddActor(PhysicsObject* _actor)
{
	if(_actor != nullptr)
		m_actors.push_back(_actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* _actor)
{
	if (_actor == nullptr)
		return;

	auto found = std::find(m_actors.begin(), m_actors.end(), _actor);

	if (found != m_actors.end())
	{
		m_actors.erase(found);
	}

	// **std::remove** is a better time complexity than vector::erase
	// goes from O(n^2) -> O(n)
	//std::remove(m_actors.begin(), m_actors.end(), _actor);
}

void PhysicsScene::Update(float _dt)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += _dt;

	while (accumulatedTime >= m_timeStep)
	{
		// Optimiseable
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

#ifndef SimulatingRocket
		int actorCount = m_actors.size();

		for (int outer = 0; outer < actorCount - 1; outer++)
		{
			for (int inner = outer + 1; inner < actorCount; inner++)
			{
				PhysicsObject* obj1 = m_actors[outer];
				PhysicsObject* obj2 = m_actors[inner];
				int shapeID1 = obj1->GetShapeID();
				int shapeID2 = obj2->GetShapeID();

				// Use function pointers
				int fnIndex = (shapeID1 * ShapeType::SHAPE_COUNT) + shapeID2;
				fn collisionFunctionPtr = collisionFunctionArray[fnIndex];
				if (collisionFunctionPtr != nullptr)
				{
					collisionFunctionPtr(obj1, obj2);
				}
			}
		}
#endif // !SimulatingRocket
	}
	GetTotalEnergy();
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw(1);
	}
}

float PhysicsScene::GetTotalEnergy()
{
	float total = 0;
	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		PhysicsObject* obj = *it;
		total += obj->GetEnergy();
	}
	return total;
}

bool PhysicsScene::Plane2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	return Circle2Plane(_obj2, _obj1);
}

bool PhysicsScene::Circle2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	Circle* circle = dynamic_cast<Circle*>(_obj1);
	Plane* plane = dynamic_cast<Plane*>(_obj2);

	if (circle != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float circToPlane = glm::dot(circle->GetPos(), plane->GetNormal());

		float intersection = circToPlane - circle->GetRadius() - plane->GetDistance();
		float velocityOutPlane = glm::dot(circle->GetVel(), plane->GetNormal());

		if (intersection < 0 && velocityOutPlane < 0)
		{
			glm::vec2 contact = circle->GetPos() + (collisionNormal * -circle->GetRadius());

			plane->ResolveCollision(circle, contact);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Circle2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	// attempt at casting objs to circles
	Circle* circ1 = dynamic_cast<Circle*>(_obj1);
	Circle* circ2 = dynamic_cast<Circle*>(_obj2);

	// if successful, test for collision
	if (circ1 != nullptr && circ2 != nullptr)
	{
		if (glm::distance(circ1->GetPos(), circ2->GetPos())
			<= circ1->GetRadius() + circ2->GetRadius())
		{
			circ1->ResolveCollision(circ2, 0.5f * (circ1->GetPos() + circ2->GetPos()));
			return true;
		}
	}
	return false;
}
