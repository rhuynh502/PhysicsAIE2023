#include "TriggerZone.h"

TriggerZone::TriggerZone(glm::vec2 _pos, glm::vec2 _vel,
	float _mass, float _width, float _height,
	glm::vec4 _color, float _multiplier) : Box(_pos, _vel, _mass, _width, _height, _color)
{
	m_multiplier = _multiplier;
}

TriggerZone::~TriggerZone()
{
}
