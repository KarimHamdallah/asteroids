#pragma once
#include <vector>
#include <engine/math/math.h>
#include <engine/renderer/renderer.h>
#include "../systems/particle_system.h"

class polygonline
{
public:
	polygonline(const std::vector<glm::vec2>& local_vertices, const glm::vec2& pos, f32 raduis, f32 rotation)
		: local_vertices(local_vertices), position(pos), raduis(raduis), rotation(rotation),
		transformationUpdateRequired(true)
	{
		transformed_vertices.reserve(local_vertices.size());
		for (size_t i = 0; i < local_vertices.size(); i++)
			transformed_vertices.emplace_back(math::transform_vector(local_vertices[i], position, glm::vec2(raduis), rotation));
	}

	~polygonline()
	{
		local_vertices.clear();
		local_vertices.shrink_to_fit();
		transformed_vertices.clear();
		transformed_vertices.shrink_to_fit();
	}

	void update()
	{
		update_transformations();
	}

	void render(const color& color, f32 lineWidth)
	{
		renderer::render_polygonline(transformed_vertices, color, lineWidth);
	}


	void setpos(const glm::vec2& new_pos) { position = new_pos; transformationUpdateRequired = true; }
	void setpos_x(f32 new_x) { position.x = new_x; transformationUpdateRequired = true; }
	void setpos_y(f32 new_y) { position.y = new_y; transformationUpdateRequired = true; }
	void move(const glm::vec2& velocity) { position += velocity; transformationUpdateRequired = true; }
	void setraduis(f32 new_raduis) { raduis = new_raduis; transformationUpdateRequired = true; }
	void setangle(f32 new_angle) { rotation = new_angle; transformationUpdateRequired = true; }

	glm::vec2 getpos() { return position; }
	f32 getraduis() { return raduis; }
	f32 getangle() { return rotation; }

	u32 getverticesCount() { return local_vertices.size(); }
	std::vector<glm::vec2> getTransformedVertices() { return transformed_vertices; }


private:
	std::vector<glm::vec2> local_vertices;
	std::vector<glm::vec2> transformed_vertices;

	glm::vec2 position;
	f32 raduis;
	f32 rotation;

	bool transformationUpdateRequired;

	void update_transformations()
	{
		if (transformationUpdateRequired)
		{
			for (size_t i = 0; i < local_vertices.size(); i++)
				transformed_vertices[i] = math::transform_vector(local_vertices[i], position, glm::vec2(raduis), rotation);
			
			transformationUpdateRequired = false;
		}
	}
};

class Bullet
{
public:

	Bullet(const glm::vec2& position, const glm::vec2& direction, f32 move_speed)
		: pos(position), dir(direction)
	{
		dir *= move_speed;
	}

	~Bullet() {}

	glm::vec2 pos;
	glm::vec2 dir;
};

class space_ship
{
public:
	space_ship(const glm::vec2& pos, f32 scale, f32 rotation);
	~space_ship();
	void update(f32 rotation_speed);
	void render(const color& ship_color, const color& bullet_color);

	std::shared_ptr<polygonline> polygon;
	std::vector<std::shared_ptr<polygonline>> lives;
	std::vector<std::shared_ptr<Bullet>> Bullets;

	void Kill();

	bool is_respawining;
	bool shield_on;
	f32 respawn_time;
	f32 shield_time;
private:
	glm::vec2 velocity;
	glm::vec2 acceleration;
	f32 move_force;
	f32 drag;
	particle_system smoke;
};