#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include <glm/glm.hpp>
#include <vector>


class PachinkoApp : public aie::Application {
public:

	PachinkoApp();
	virtual ~PachinkoApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	std::vector<PhysicsObject*> m_objects;

public:
	float DegreesToRadians(float _degrees);
	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);

	const float m_extents = 100;
	const float m_aspectRatio = 9.0f / 16.0f;

	float m_cameraX;
	float m_cameraY;

	float m_totPoints;
	float m_pointBet;
	
};