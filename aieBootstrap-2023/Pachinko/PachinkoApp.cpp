#include "PachinkoApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Circle.h"
#include "Box.h"
#include "Variables.h"
#include "Gizmos.h"
#include "PachinkoBall.h"
#include "TriggerZone.h"
#include <glm/ext.hpp>

#include <iostream>


PachinkoApp::PachinkoApp() {
	m_cameraX = 0;
	m_cameraY = 0;
	m_totPoints = 10000;
	m_pointBet = 1;
}

PachinkoApp::~PachinkoApp() 
{

}

bool PachinkoApp::startup() {
	
	aie::Gizmos::create(8000U, 8000U, 8000U, 8000U);

	m_2dRenderer = new aie::Renderer2D();

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01);

	m_physicsScene->SetGravity(glm::vec2(0, -10));

	// Walls
	float windowWidth = getWindowWidth();
	float windowHeight = getWindowHeight();

	Box* wallLeft = new Box(glm::vec2(-100, 0), glm::vec2(0), 100, 10, windowHeight, glm::vec4(1, 1, 1, 1));
	Box* wallRight = new Box(glm::vec2(100, 0), glm::vec2(0), 100, 10, windowHeight, glm::vec4(1, 1, 1, 1));

	wallLeft->SetKinematic(true);
	wallLeft->SetStaticFriction(0);
	wallLeft->SetKineticFriction(0);
	wallLeft->SetElasticity(0.5f);
	wallRight->SetKinematic(true);
	wallRight->SetStaticFriction(0);
	wallRight->SetKineticFriction(0);
	wallRight->SetElasticity(1.f);

	m_physicsScene->AddActor(wallLeft);
	m_physicsScene->AddActor(wallRight);

	// Pin Layers
	for (int i = 0; i < 10; i++)
	{
		for (int j = i % 2 ? 10 : 11; j > 0; j--)
		{
			Circle* pin = new Circle(glm::vec2(-41.5f + (7 * j) + (3.5f * (i % 2)), 50 - (6 * i)), glm::vec2(0), 1, 0.8f, glm::vec4(1, 1, 1, 1));
			pin->SetKinematic(true);
			pin->SetStaticFriction(0.3299f);
			pin->SetKineticFriction(0.2677f);
			pin->SetElasticity(0.1872f);

			m_physicsScene->AddActor(pin);
		}
	}
	// Top Pins Left and right

	for (int i = -1; i < 2; i += 2)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = j % 2 ? 6 : 7; k > 0; k--)
			{
				Circle* pin = new Circle(glm::vec2((60 * i) + (-7 * i * k) + (-3.5f * (j % 2) * i), 100 - (4 * j)), glm::vec2(0), 1, 0.8f, glm::vec4(1, 1, 1, 1));
				pin->SetKinematic(true);
				pin->SetStaticFriction(0.3299f);
				pin->SetKineticFriction(0.2677f);
				pin->SetElasticity(0.1872f);

				m_physicsScene->AddActor(pin);
			}
		}
	}

	// Mid Pins Left and Right
	for (int i = -1; i < 2; i += 2)
	{
		for (int j = 0; j < 7; j++)
		{
			for (int k = j % 2 ? 3 : 4; k > 0; k--)
			{
				Circle* pin = new Circle(glm::vec2((95 * i) + (-7 * i * k) + (-3.5f * (j % 2) * i), 25 - (4 * j)), glm::vec2(0), 1, 0.8f, glm::vec4(1, 1, 1, 1));
				pin->SetKinematic(true);
				pin->SetStaticFriction(0.3299f);
				pin->SetKineticFriction(0.2677f);
				pin->SetElasticity(0.1872f);

				m_physicsScene->AddActor(pin);
			}
		}
	}

	// Pins at Bottom
	for (int i = 0; i < 4; i++)
	{
		for (int j = i % 2 ? 20 : 21; j > 0; j--)
		{
			Circle* pin = new Circle(glm::vec2(-94.f + (8.5f * j) + (3.5f * (i % 2)), -40 - (6 * i)), glm::vec2(0), 1, 0.8f, glm::vec4(1, 1, 1, 1));
			pin->SetKinematic(true);
			pin->SetStaticFriction(0.3299f);
			pin->SetKineticFriction(0.2677f);
			pin->SetElasticity(0.1872f);

			m_physicsScene->AddActor(pin);
		}
	}

	// Ramps
	// Ramps at the top
	for (int i = -1; i < 2; i += 2)
	{
		Box* ramp = new Box(glm::vec2(i * 50, 149), glm::vec2(0), 100, 98.2f, 1, glm::vec4(1, 1, 1, 1));
		ramp->SetKinematic(true);
		ramp->SetStaticFriction(0);
		ramp->SetKineticFriction(0);
		ramp->SetElasticity(0.0721f);
		ramp->SetOrientation(DegreesToRadians(i * 11));

		m_physicsScene->AddActor(ramp);
	}

	// Splitter Ramps
	for (int i = 0; i < 5; i++)
	{
		Box* ramp = new Box(glm::vec2(-12 + (i * 6.5f), 125 - (i * 3)), glm::vec2(0), 100, 35 - (i * 7), 1, glm::vec4(1, 1, 1, 1));
		ramp->SetKinematic(true);
		ramp->SetStaticFriction(0.1214f);
		ramp->SetKineticFriction(0.0635f);
		ramp->SetElasticity(0.0721f);
		ramp->SetOrientation(DegreesToRadians(45));

		m_physicsScene->AddActor(ramp);
	}

	// Low Elasticity Mid Friction
	for (int i = -1; i < 2; i += 2)
	{
		Box* ramp = new Box(glm::vec2(i * 40, -20), glm::vec2(0), 100, 35, 1, glm::vec4(1, 1, 1, 1));
		ramp->SetOrientation(DegreesToRadians(i * 15));
		ramp->SetKinematic(true);
		ramp->SetStaticFriction(0.2721f);
		ramp->SetKineticFriction(0.2015f);
		ramp->SetElasticity(0.1126f);

		m_physicsScene->AddActor(ramp);
	}

	// Ramps next to pins
	for (int i = -1; i < 2; i += 2)
	{
		Box* ramp = new Box(glm::vec2(i * 55, 50), glm::vec2(0), 100, 30, 1, glm::vec4(1, 1, 1, 1));
		ramp->SetOrientation(DegreesToRadians(i * -30));
		ramp->SetKinematic(true);
		ramp->SetStaticFriction(0.2421f);
		ramp->SetKineticFriction(0.1715f);
		ramp->SetElasticity(0.1126f);

		m_physicsScene->AddActor(ramp);
	}

	// Trigger Area
	Box* floor = new Box(glm::vec2(0, -71), glm::vec2(0), 100, 200, 1, glm::vec4(1, 1, 1, 1));
	floor->SetKinematic(true);
	floor->SetElasticity(0.3f);

	m_physicsScene->AddActor(floor);

	for (int i = 0; i < 8; i++)
	{
		Box* stopper = new Box(glm::vec2(-100.f + (i * 25), -68.5f), glm::vec2(0), 100, 1, 6, glm::vec4(1, 1, 1, 1));
		stopper->SetKinematic(true);
		stopper->SetElasticity(0.2f);

		m_physicsScene->AddActor(stopper);
	}

	for (int i = 0; i < 8; i++)
	{
		float mult;
		if (i == 0 || i == 3)
			mult = 0.5f;
		if (i == 4)
			mult = 1.f;
		if (i == 2 || i == 5)
			mult = 1.5f;
		if (i == 1 || i == 7)
			mult = 2.f;
		if (i == 6)
			mult = 3.f;

		TriggerZone* trigger = new TriggerZone(glm::vec2(-87.5f + (i * 25), -68.6f), glm::vec2(0), 100, 24, 6, glm::vec4(1, 1, 1, 0.5f), mult);
		trigger->SetTrigger(true);
		trigger->SetKinematic(true);

		trigger->triggerEnter = [=](PhysicsObject* _other)
		{
			PachinkoBall* ball = dynamic_cast<PachinkoBall*>(_other);
			if (ball)
			{
				m_totPoints += ball->GetPoints() * trigger->GetMultiplier();
				m_physicsScene->AddToRemove(_other);
			}
		};
		m_physicsScene->AddActor(trigger);
	}

	m_font = new aie::Font("./font/consolas.ttf", 32);

	return true;
}

void PachinkoApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PachinkoApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);

	if (input->isKeyDown(aie::INPUT_KEY_1))
	{
		m_pointBet = 1;
	}
	if (input->isKeyDown(aie::INPUT_KEY_2))
	{
		m_pointBet = 10;
	}
	if (input->isKeyDown(aie::INPUT_KEY_3))
	{
		m_pointBet = 100;
	}
	if (input->isKeyDown(aie::INPUT_KEY_4))
	{
		m_pointBet = 1000;
	}

	if (input->wasMouseButtonPressed(0))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
		if (yScreen > 910 && xScreen < 520 && xScreen > 20 && m_totPoints >= m_pointBet)
		{
			PachinkoBall* ball = new PachinkoBall(worldPos, glm::vec2(0), 5, 1.f, glm::vec4(1, 0, 1, 1), m_pointBet);
			ball->SetElasticity(0.2f);
			ball->SetStaticFriction(0.1856f);
			ball->SetKineticFriction(0.1024f);
			m_physicsScene->AddActor(ball);

			m_totPoints -= m_pointBet;
		}
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PachinkoApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	m_physicsScene->Draw();

	// draw your stuff here!
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));

	m_2dRenderer->setRenderColour(1, 0, 0, 1);
	m_2dRenderer->drawLine(0, 910, 540, 910);
	m_2dRenderer->setRenderColour(1, 1, 1, 1);

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 20, 0);
	m_2dRenderer->drawText(m_font, std::to_string(m_totPoints).c_str(), 20, 150);
	m_2dRenderer->drawText(m_font, "0.5", 15, 265);
	m_2dRenderer->drawText(m_font, "2", 92.5, 265);
	m_2dRenderer->drawText(m_font, "1.5", 145, 265);
	m_2dRenderer->drawText(m_font, "0.5", 210, 265);
	m_2dRenderer->drawText(m_font, "1", 295, 265);
	m_2dRenderer->drawText(m_font, "1.5", 345, 265);
	m_2dRenderer->drawText(m_font, "3", 430, 265);
	m_2dRenderer->drawText(m_font, "2", 490, 265);
	m_2dRenderer->drawText(m_font, "Current Bet:", 20, 75);
	m_2dRenderer->drawText(m_font, std::to_string((int)m_pointBet).c_str(), 250, 75);

	// done drawing sprites
	m_2dRenderer->end();
}

float PachinkoApp::DegreesToRadians(float _degrees)
{
	return _degrees * (PI / 180.0f);
}

glm::vec2 PachinkoApp::ScreenToWorld(glm::vec2 _screenPos)
{
	glm::vec2 worldPos = _screenPos;

	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}
