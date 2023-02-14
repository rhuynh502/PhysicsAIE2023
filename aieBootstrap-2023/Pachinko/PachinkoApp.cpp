#include "PachinkoApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Circle.h"
#include "Box.h"
#include "Variables.h"
#include "Gizmos.h"
#include <glm/ext.hpp>

#include <iostream>

#include "PhysicsScene.h"

PachinkoApp::PachinkoApp() {
	m_cameraX = 0;
	m_cameraY = 0;
}

PachinkoApp::~PachinkoApp() {

}

bool PachinkoApp::startup() {
	
	aie::Gizmos::create(450U, 450U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01);

	m_physicsScene->SetGravity(glm::vec2(0, -10));

	// Walls
	float windowWidth = getWindowWidth();
	float windowHeight = getWindowHeight();

	Box* wallLeft = new Box(glm::vec2(-100, 0), glm::vec2(0), 100, 15, windowHeight, glm::vec4(1, 1, 1, 1));
	Box* wallRight = new Box(glm::vec2(100, 0), glm::vec2(0), 100, 15, windowHeight, glm::vec4(1, 1, 1, 1));

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
	for (int i = 0; i < 11; i++)
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
			for (int k = j % 2 ? 11 : 12; k > 0; k--)
			{
				Circle* pin = new Circle(glm::vec2((70 * i) + (-5 * i * k) + (2 * (j % 2)), 80 - (4 * j)), glm::vec2(0), 1, 0.8f, glm::vec4(1, 1, 1, 1));
				pin->SetKinematic(true);
				pin->SetStaticFriction(0.3299f);
				pin->SetKineticFriction(0.2677f);
				pin->SetElasticity(0.1872f);

				m_physicsScene->AddActor(pin);
			}
		}
	}

	// Ramps

	/*Circle* testBox = new Circle(glm::vec2(-50, 0), glm::vec2(0), 100, 20, glm::vec4(1, 1, 1, 1));
	testBox->SetKinematic(true);
	m_physicsScene->AddActor(testBox);*/

	// Ramps at the top
	for (int i = -1; i < 2; i += 2)
	{
		Box* ramp = new Box(glm::vec2(i * 50, 149), glm::vec2(0), 100, 95, 1, glm::vec4(1, 1, 1, 1));
		ramp->SetKinematic(true);
		ramp->SetStaticFriction(0);
		ramp->SetKineticFriction(0);
		ramp->SetElasticity(0.0721f);
		ramp->SetOrientation(DegreesToRadians(i * 9));

		m_physicsScene->AddActor(ramp);
	}

	// Splitter Ramps
	for (int i = 0; i < 4; i++)
	{
		Box* ramp = new Box(glm::vec2(-12 + (i * 6), 125 - (i * 2)), glm::vec2(0), 100, 35 - (i * 7), 1, glm::vec4(1, 1, 1, 1));
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
		Box* ramp = new Box(glm::vec2(i * 25, -20), glm::vec2(0), 100, 40, 1, glm::vec4(1, 1, 1, 1));
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
		Box* ramp = new Box(glm::vec2(i * 55, 50), glm::vec2(0), 100, 45, 1, glm::vec4(1, 1, 1, 1));
		ramp->SetOrientation(DegreesToRadians(i * -30));
		ramp->SetKinematic(true);
		ramp->SetStaticFriction(0.2421f);
		ramp->SetKineticFriction(0.1715f);
		ramp->SetElasticity(0.1126f);

		m_physicsScene->AddActor(ramp);
	}

	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

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

	if (input->wasMouseButtonPressed(0))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
		if (yScreen > 900 && xScreen < 520 && xScreen > 20)
		{
			Circle* ball = new Circle(worldPos, glm::vec2(0), 5, 1.f, glm::vec4(1, 0, 1, 1));
			ball->SetElasticity(0.2f);
			ball->SetStaticFriction(0.1856f);
			ball->SetKineticFriction(0.1024f);
			m_physicsScene->AddActor(ball);
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
	m_2dRenderer->drawLine(0, 900, 540, 900);
	m_2dRenderer->setRenderColour(1, 1, 1, 1);

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

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
