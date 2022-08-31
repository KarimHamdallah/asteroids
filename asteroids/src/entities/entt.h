#pragma once
#include <engine/defines.h>
#include <glm/glm.hpp>

class game_entity
{
public:
	game_entity() {}
	virtual ~game_entity() {}

	glm::vec2 position;
	glm::vec2 scale;
	f32 rotation;
};