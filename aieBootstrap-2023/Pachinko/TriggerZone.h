#pragma once
#include "Box.h"

class TriggerZone : public Box
{
public:
	TriggerZone(glm::vec2 _pos, glm::vec2 _vel,
		float _mass, float _width, float _height,
		glm::vec4 _color,
		float _multiplier);
	~TriggerZone();

	float GetMultiplier() { return m_multiplier; }

protected:
	float m_multiplier;
};

