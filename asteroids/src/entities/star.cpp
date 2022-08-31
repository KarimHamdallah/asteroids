#include "star.h"
#include <snap_engine.h>

star::star(const glm::vec2& position, f32 scale, f32 rotation)
{
	this->position = position;
	this->scale = glm::vec2(scale);
	this->rotation = rotation;
	move_dir = glm::vec2(random(-1.0f, 1.0f), random(-1.0f, 1.0f));
	move_force = 2.0f;
	

	f32 angle = 0.0f;
	u32 vertices_count = 8;

	vertices.reserve(vertices_count);
	transformed_vertices.reserve(vertices_count);

	for (size_t i = 0; i < vertices_count; i++)
	{
		glm::vec2 point = math::get_vector(random(0.3f, 1.0f), angle);
		vertices.emplace_back(point);

		angle += 360 / vertices_count;
	}


	for (size_t i = 0; i < vertices.size(); i++)
	{
		transformed_vertices.emplace_back(math::transform_vector(vertices[i], this->position, this->scale, this->rotation));
	}
}

star::~star()
{
}

void star::update()
{
	f32 width = window::getInstance()->getWidth();
	f32 height = window::getInstance()->getHeight();

	rotation++;
	rotation = (u32)rotation % 360;

	position += move_force * move_dir;
	if (position.x < 0.0f)
		position.x = width;
	if (position.x > width)
		position.x = 0.0f;
	if (position.y < 0.0f)
		position.y = height;
	if (position.y > height)
		position.y = 0.0f;

	update_transform();
}

void star::render()
{
	renderer::render_polygonline(transformed_vertices, WHITE, 1.2f);
}

void star::update_transform()
{
	for (size_t i = 0; i < transformed_vertices.size(); i++)
	{
		transformed_vertices[i] = math::transform_vector(vertices[i], position, scale, rotation);
	}
}