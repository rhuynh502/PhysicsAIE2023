#include "PhysicsScene.h"
#include "Circle.h"
#include "Demos.h"

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01;
	m_gravity = glm::vec2(0);
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

				Circle2Circle(obj1, obj2);
			}
		}
#endif // !SimulatingRocket

	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw(1);
	}
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
			glm::vec2 normalise = glm::normalize(circ2->GetPos() - circ1->GetPos());
			
			float impulseMag = (- 1 * (1 + 1) * glm::dot(circ1->GetVel() - circ2->GetVel(), normalise))
				/ glm::dot(normalise , normalise * ((1 / circ1->GetMass()) + (1 / circ2->GetMass())));

			glm::vec2 circ1Final = circ1->GetVel() + (impulseMag / circ1->GetMass()) * normalise;
			glm::vec2 circ2Final = circ2->GetVel() - (impulseMag / circ2->GetMass()) * normalise;

			circ1->SetVel(circ1Final);
			circ2->SetVel(circ2Final);
		}
	}

	return false;
}
