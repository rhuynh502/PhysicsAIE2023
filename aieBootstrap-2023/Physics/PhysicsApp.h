#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"
#include "Demos.h"
#include "Circle.h"
#include <vector>

class PhysicsScene;
class PhysicsObject;

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();

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
	void DemoStartUp(int _demoNumber);
	void DemoUpdate(aie::Input* _input, float _dt);
	float DegreesToRadians(float _degrees);
	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	float m_cameraX;
	float m_cameraY;

#ifdef SimulatingRocket
	Circle* m_rocket;
	int m_timer;
	std::vector<Circle*> m_fuel;

	void GetFuel();
	void ApplyFuel(Circle* _fuel, float _angle);
#endif

#ifdef SetPoolTable
	Circle* m_cueBall;
	glm::vec2 m_appliedForce = glm::vec2(0);
	glm::vec2 m_initialPos;
	float m_score = 0;
	bool m_onClick = false;
#endif
};