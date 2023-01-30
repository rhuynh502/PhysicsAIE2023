#include "PhysicsScene.h"

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01;
	m_gravity = glm::vec2(0);
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

	// **std::remove** is a better time complexity
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
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw(1);
	}
}
