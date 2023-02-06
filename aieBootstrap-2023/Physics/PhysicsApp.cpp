#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Demos.h"
#include "Circle.h"
#include "Plane.h"
#include "Box.h"

#include "PhysicsScene.h"

#include <glm/ext.hpp>

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 6535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01);

	DemoStartUp(1);

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	DemoUpdate(input, deltaTime);

#ifdef SimulatingRocket

	if (input->isKeyDown(aie::INPUT_KEY_W) && m_rocket->GetMass() > 15 && m_timer > 1)
	{
		GetFuel();
		
	}
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_rocket->SetOrientation(m_rocket->GetOrientation() + 1);
	}
	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_rocket->SetOrientation(m_rocket->GetOrientation() - 1);
	}

	m_timer++;
#endif

#ifdef SetPoolTable
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		if (!m_onClick)
		{
			m_initialPos.x = input->getMouseX();
			m_initialPos.y = input->getMouseY();
			m_onClick = true;
		}
		m_appliedForce.x = m_initialPos.x - input->getMouseX();
		m_appliedForce.y = m_initialPos.y - input->getMouseY();

		aie::Gizmos::add2DLine(m_cueBall->GetPos(), m_cueBall->GetPos() - (-m_appliedForce / 10.f), glm::vec4(1, 0, 0, 1));
	}
	if (input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		m_cueBall->ApplyForce(m_appliedForce, m_cueBall->GetPos());
		m_onClick = false;
		m_initialPos = glm::vec2(0);
		m_appliedForce = glm::vec2(0);
	}

#endif // launch cue ball
	
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::DemoStartUp(int _demoNumber)
{
#ifdef NewtonsFirstLaw
	m_physicsScene->SetGravity(glm::vec2(0, 0));
	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 
		3.f, 1, glm::vec4(1, 0, 1, 1));
	m_physicsScene->AddActor(ball);
#endif // Newtons first law

#ifdef NewtonsSecondLaw
	m_physicsScene->SetGravity(glm::vec2(0, -9.8));
	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30),
		3.f, 1, glm::vec4(1, 0, 1, 1));
	m_physicsScene->AddActor(ball);
#endif // Newtons second law

#ifdef NewtonsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(-4, 0), glm::vec2(0, 0),
		4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(4, 0), glm::vec2(0, 0),
		4.0f, 4, glm::vec4(1, 0, 1, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForceToActor(ball2, glm::vec2(-2, 0));
#endif // Newtons third law

#ifdef SimulatingCollision
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0, 0),
		4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0, 0),
		4.0f, 4, glm::vec4(1, 0, 1, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(5, 0), ball1->GetPos());
	ball2->ApplyForce(glm::vec2(-3, 0), ball2->GetPos());
#endif // Simulating Collision

#ifdef SimulatingRocket
	m_physicsScene->SetGravity(glm::vec2(0, -0.f));

	m_rocket = new Circle(glm::vec2(0, 0), glm::vec2(0, 0),
		100.f, 6, glm::vec4(1, 0, 1, 1));

	m_rocket->SetOrientation(-90);

	m_physicsScene->AddActor(m_rocket);
	m_timer = 0;
#endif // Simulating Rocket

#ifdef CircleToPlane
	m_physicsScene->SetGravity(glm::vec2(0, -9.8f));

	Circle* ball1 = new Circle(glm::vec2(-20, 20), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 20), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 1, 1));
	Plane* plane = new Plane(glm::vec2(0, 1), -50.f, glm::vec4(0, 1, 0, 1));
	Plane* plane1 = new Plane(glm::vec2(1, 0), -60.f, glm::vec4(1, 1, 1, 1));
	Plane* plane2 = new Plane(glm::normalize(glm::vec2(4, 7)), -20.f, glm::vec4(1, 1, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(0, -1), -50.f, glm::vec4(1, 1, 1, 1));
	Plane* plane4 = new Plane(glm::vec2(-1, 0), -50.f, glm::vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(plane);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(plane4);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	//ball1->ApplyForce(glm::vec2(0, 0));

#endif // Plane Test

#ifdef CircleToCircleNewtons
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(-30, 0), glm::vec2(6, 0), 5, 3, glm::vec4(1, 0, 1, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0, 0), 5, 3, glm::vec4(1, 0, 1, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

#endif // Newtons Cradle

#ifdef SymmetricalNewtons
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 8, 3, glm::vec4(1, 0, 1, 1));
	Circle* ball2 = new Circle(glm::vec2(6, 0), glm::vec2(0, 0), 8, 3, glm::vec4(1, 0, 1, 1));
	Circle* ball3 = new Circle(glm::vec2(12, 0), glm::vec2(0, 0), 8, 3, glm::vec4(1, 0, 1, 1));
	Circle* ball4 = new Circle(glm::vec2(-6, 0), glm::vec2(0, 0), 8, 3, glm::vec4(1, 0, 1, 1));
	Circle* ball5 = new Circle(glm::vec2(-12, 0), glm::vec2(0, 0), 8, 3, glm::vec4(1, 0, 1, 1));

	Plane* wall1 = new Plane(glm::vec2(1, 0), -20, glm::vec4(1, 1, 1, 1));
	Plane* wall2 = new Plane(glm::vec2(-1, 0), -20, glm::vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(ball1); 
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(wall1);
	m_physicsScene->AddActor(wall2);

	ball3->ApplyForce(glm::vec2(-65, 0), ball3->GetPos());

#endif // Symmetrical Newtons Cradle

#ifdef AsymmetricalNewtons
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 16, 3, glm::vec4(1, 1, 1, 1));
	Circle* ball2 = new Circle(glm::vec2(6, 0), glm::vec2(0, 0), 8, 3, glm::vec4(1, 0, 1, 1));
	Circle* ball3 = new Circle(glm::vec2(12, 0), glm::vec2(0, 0), 8, 3, glm::vec4(1, 0, 1, 1));
	Circle* ball4 = new Circle(glm::vec2(-6, 0), glm::vec2(0, 0), 8, 3, glm::vec4(1, 0, 1, 1));
	Circle* ball5 = new Circle(glm::vec2(-12, 0), glm::vec2(0, 0), 8, 3, glm::vec4(1, 0, 1, 1));

	Plane* wall1 = new Plane(glm::vec2(1, 0), -20, glm::vec4(1, 1, 1, 1));
	Plane* wall2 = new Plane(glm::vec2(-1, 0), -20, glm::vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(ball1); 
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(wall1);
	m_physicsScene->AddActor(wall2);

	ball3->ApplyForce(glm::vec2(-65, 0));

#endif // Symmetrical Newtons Cradle

#ifdef BoxOnPlane
	m_physicsScene->SetGravity(glm::vec2(0, -10));

	Plane* floor = new Plane(glm::vec2(0, 1), -30, glm::vec4(1, 1, 1, 1));

	Circle* ball = new Circle(glm::vec2(20, 0), glm::vec2(0, 0), 4.f, 4.f, glm::vec4(1, 0, 1, 1));
	Box* boxFlat = new Box(glm::vec2(-20, 0), glm::vec2(0, 0), 4.f, glm::vec2(4, 4), glm::vec4(1, 0, 1, 1));
	Box* boxAngle = new Box(glm::vec2(0, 0), glm::vec2(0, 0), 4.f, glm::vec2(4, 4), glm::vec4(1, 0, 1, 1));
	boxAngle->SetOrientation(DegreesToRadians(30));

	m_physicsScene->AddActor(floor);
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(boxFlat);
	m_physicsScene->AddActor(boxAngle);

#endif // Falling box on plane

#ifdef BoxOnCircle
	m_physicsScene->SetGravity(glm::vec2(0, -10));

	Plane* floor = new Plane(glm::vec2(0, 1), -15, glm::vec4(1, 1, 1, 1));
	Plane* floor1 = new Plane(glm::vec2(1, 0), -15, glm::vec4(1, 1, 1, 1));
	Plane* floor2 = new Plane(glm::vec2(0, -1), -15, glm::vec4(1, 1, 1, 1));
	Plane* floor3 = new Plane(glm::vec2(-1, 0), -15, glm::vec4(1, 1, 1, 1));
	Circle* ball = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 4.f, 2.f, glm::vec4(1, 0, 1, 1));
	Circle* ball1 = new Circle(glm::vec2(3, 3), glm::vec2(0, 0), 4.f, 2.f, glm::vec4(1, 0, 1, 1));
	Circle* ball2 = new Circle(glm::vec2(5, 5), glm::vec2(0, 0), 4.f, 2.f, glm::vec4(1, 0, 1, 1));
	Box* boxFlat = new Box(glm::vec2(-2, -4), glm::vec2(0, 0), 4.f, glm::vec2(2, 2), glm::vec4(1, 0, 1, 1));
	Box* boxAngle = new Box(glm::vec2(-6, 2), glm::vec2(0, 0), 4.f, glm::vec2(2, 2), glm::vec4(1, 0, 1, 1));

	m_physicsScene->AddActor(floor);
	m_physicsScene->AddActor(floor1);
	m_physicsScene->AddActor(floor2);
	m_physicsScene->AddActor(floor3);
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(boxAngle);
	m_physicsScene->AddActor(boxFlat);

	ball1->ApplyForce(glm::vec2(32, 32), ball1->GetPos());

#endif // box on circle collision

#ifdef BounceToStop
	m_physicsScene->SetGravity(glm::vec2(0, -10));

	Plane* floor = new Plane(glm::vec2(0, 1), -15, glm::vec4(1, 1, 1, 1));
	Circle* ball = new Circle(glm::vec2(5, 8), glm::vec2(0, 0), 4.f, 2.f, glm::vec4(1, 0, 1, 1));
	Box* boxFlat = new Box(glm::vec2(-5, 8), glm::vec2(0, 0), 4.f, glm::vec2(2, 2), glm::vec4(1, 0, 1, 1));

	ball->SetElasticity(0.4f);
	boxFlat->SetElasticity(0.5f);
	floor->SetElasticity(0.7f);

	m_physicsScene->AddActor(floor);
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(boxFlat);

#endif

#ifdef SetPoolTable
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Plane* floor = new Plane(glm::vec2(0, 1), -28, glm::vec4(1, 1, 1, 1));
	Plane* floor1 = new Plane(glm::vec2(1, 0), -56, glm::vec4(1, 1, 1, 1));
	Plane* floor2 = new Plane(glm::vec2(0, -1), -28, glm::vec4(1, 1, 1, 1));
	Plane* floor3 = new Plane(glm::vec2(-1, 0), -56, glm::vec4(1, 1, 1, 1));

	floor->SetElasticity(0.5f);
	floor1->SetElasticity(0.5f);
	floor2->SetElasticity(0.5f);
	floor3->SetElasticity(0.5f);

	for (int i = 0; i <= 4; i++)
	{
		int k = 0;
		for (int j = i + 1; j > 0; j--)
		{
			Circle* ball = new Circle(glm::vec2(-30 + (i * -2), i + k), glm::vec2(0), 7, 1, glm::vec4(0, 1, 0, 1));

			ball->SetElasticity(0.8f);

			m_physicsScene->AddActor(ball);
			k -= 2;
		}
	}

	m_cueBall = new Circle(glm::vec2(45, 0), glm::vec2(0, 0), 5, 0.8f, glm::vec4(1, 0, 1, 1));
	m_cueBall->SetElasticity(0.7f);

	m_physicsScene->AddActor(floor);
	m_physicsScene->AddActor(floor1);
	m_physicsScene->AddActor(floor2);
	m_physicsScene->AddActor(floor3);

	m_physicsScene->AddActor(m_cueBall);
#endif // basic pool table situation
}

#ifdef SimulatingRocket

void PhysicsApp::GetFuel()
{
	float radius = m_rocket->GetRadius();
	glm::vec2 currPos = m_rocket->GetPos();
	float angle = DegreesToRadians(m_rocket->GetOrientation());

	if (m_fuel.size() < 10)
	{
		Circle* fuel = new Circle(glm::vec2(currPos.x + (radius + 0.5f) * glm::cos(angle),
			currPos.y + (radius + 0.5f) * glm::sin(angle)), glm::vec2(0),
			1.f, .5f, glm::vec4(0, 1, 0, 1));

		m_physicsScene->AddActor(fuel);
		
		ApplyFuel(fuel, angle);
		
	}
	else
	{
		Circle* fuel = m_fuel.front();
		m_fuel.erase(m_fuel.begin());
		fuel->SetPos(glm::vec2(currPos.x + (radius + 0.5f) * glm::cos(angle),
			currPos.y + (radius + 0.5f) * glm::sin(angle)));
		fuel->SetVel(glm::vec2(0));
		ApplyFuel(fuel, angle);
	}


	m_timer = 0;
}

void PhysicsApp::ApplyFuel(Circle* _fuel, float _angle)
{
	m_fuel.push_back(_fuel);
	m_rocket->SetMass(m_rocket->GetMass() - _fuel->GetMass());
	_fuel->ApplyForceToActor(m_rocket, glm::vec2(1.f * glm::cos(_angle), 1.f * glm::sin(_angle)));
}
#endif // functions for rocket simulation

void PhysicsApp::DemoUpdate(aie::Input* _input, float _dt)
{
#ifdef NewtonsThirdLaw
	if (_input->isKeyDown(aie::INPUT_KEY_SPACE))
	{
	}
#endif // Newtons third law
}

float PhysicsApp::DegreesToRadians(float _degrees)
{
	return _degrees * (PI / 180.f);
}
