#pragma once
#include "entt.h"
#include <glm/glm.hpp>
#include <vector>

class star : public game_entity
{
public:
	star(const glm::vec2& position, f32 scale, f32 rotation);
	~star();

	void update();
	void render();


	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> transformed_vertices;
private:
	glm::vec2 move_dir;
	f32 move_force;


	void update_transform();
};