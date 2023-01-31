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

#ifdef SimulatingRocket
	Circle* m_rocket;
	int m_timer;
	std::vector<Circle*> m_fuel;

	void GetFuel();
	void ApplyFuel(Circle* _fuel, float _angle);
#endif
};