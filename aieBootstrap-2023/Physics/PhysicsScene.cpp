#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"
#include "Demos.h"
#include "Box.h"
#include <iostream>

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

glm::vec2 PhysicsScene::m_gravity(0, 0);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle, PhysicsScene::Plane2Box,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle, PhysicsScene::Circle2Box,
	PhysicsScene::Box2Plane, PhysicsScene::Box2Circle, PhysicsScene::Box2Box,
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

		CheckForCollision();

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
				int fnIndex = (shapeID1 * SHAPE_COUNT) + shapeID2;
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

void PhysicsScene::CheckForCollision()
{
	int actorCount = m_actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* obj1 = m_actors[outer];
			PhysicsObject* obj2 = m_actors[inner];
			int shapeId1 = obj1->GetShapeID();
			int shapeId2 = obj2->GetShapeID();

			if (shapeId1 < 0 || shapeId2 < 0)
				continue;
		}
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

void PhysicsScene::ApplyContactForces(RigidBody* _body1, RigidBody* _body2, glm::vec2 _norm, float _pen)
{
	float body2Mass = _body2 ? _body2->GetMass() : INT_MAX;

	float body1Factor = body2Mass / (_body1->GetMass() + body2Mass);

	_body1->SetPos(_body1->GetPos() - body1Factor * _norm * _pen);
	if (_body2)
		_body2->SetPos(_body2->GetPos() + (1 - body1Factor) * _norm * _pen);

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
		float penetration = circ1->GetRadius() + circ2->GetRadius() - glm::distance(circ1->GetPos(), circ2->GetPos());
		if (penetration > 0)
		{
			circ1->ResolveCollision(circ2, 0.5f * (circ1->GetPos() + circ2->GetPos()), nullptr, penetration);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2Box(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	return Box2Circle(_obj2, _obj1);
}

bool PhysicsScene::Box2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	Box* box = dynamic_cast<Box*>(_obj1);
	Plane* plane = dynamic_cast<Plane*>(_obj2);

	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;

		glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

		// check all corners for collision
		for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
		{
			for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
			{
				// Get pos of corner in world space
				glm::vec2 p = box->GetPos() + x * box->GetLocalX() + y * box->GetLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());

				// total vel of point in world space
				glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
				glm::vec2 pointVelocity = box->GetVel() + box->GetAngularVel() * glm::vec2(-displacement.y, displacement.x);
				float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());

				if (distFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}

		if (numContacts > 0)
		{
			plane->ResolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Box2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	Box* box = dynamic_cast<Box*>(_obj1);
	Circle* circle = dynamic_cast<Circle*>(_obj2);

	if (box != nullptr && circle != nullptr)
	{
		glm::vec2 circlePosWorld = circle->GetPos() - box->GetPos();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()), glm::dot(circlePosWorld, box->GetLocalY()));

		glm::vec2 closestPointOnBoxBox = circlePosBox;
		glm::vec2 extents = box->GetExtents();
		if (closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = -extents.x;
		if (closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
		if (closestPointOnBoxBox.y < -extents.y) closestPointOnBoxBox.y = -extents.y;
		if (closestPointOnBoxBox.y > extents.y) closestPointOnBoxBox.y = extents.y;

		glm::vec2 closestPointOnBoxWorld = box->GetPos() + closestPointOnBoxBox.x * box->GetLocalX() + closestPointOnBoxBox.y * box->GetLocalY();
		glm::vec2 circleToBox = circle->GetPos() - closestPointOnBoxWorld;

		float pen = circle->GetRadius() - glm::length(circleToBox);

		if (pen > 0)
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->ResolveCollision(circle, contact, &direction, pen);
			return true;
		}

	}
	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	Box* box1 = dynamic_cast<Box*>(_obj1);
	Box* box2 = dynamic_cast<Box*>(_obj2);
	if (box1 != nullptr && box2 != nullptr) {
		glm::vec2 boxPos = box2->GetPos() - box1->GetPos();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm)) {
			norm = -norm;
		}
		if (pen > 0) {
			box1->ResolveCollision(box2, contact / float(numContacts), &norm, pen);
		}
		return true;
	}
	return false;
}

bool PhysicsScene::Plane2Box(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	return Box2Plane(_obj2, _obj1);
}
