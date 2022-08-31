#include "space_ship.h"
#include <engine/renderer/renderer.h>
#include <engine/input/input.h>
#include <engine/math/math.h>
#include <engine/time/time.h>
#include <engine/logger/asserts.h>
#include "../asteroid.h"
#include "../main_menu.h"


glm::vec2 dir = glm::vec2(0.0f);
#define MOVE_FORCE 200.0f
#define BULLET_SPEED 5.0f
#define LIVES 20
#define SHIELDTIME 5
#define RESPAWNTIME 5
f32 fade = 1.0f;

space_ship::space_ship(const glm::vec2 & pos, f32 scale, f32 rotation)
{
	f32 width = window::getInstance()->getWidth();
	f32 height = window::getInstance()->getHeight();
	is_respawining = false;
	respawn_time = 0.0f;
	shield_on = true;
	shield_time = 0.0f;

	velocity = glm::vec2(0.0f);
	acceleration = glm::vec2(0.0f);
	move_force = MOVE_FORCE;
	drag = 0.99f;

	// set vertices in local space
	std::vector<glm::vec2> local_vertices =
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(-0.5f, -0.5f),
		glm::vec2(0.0f, -0.2f),
		glm::vec2(0.5f, -0.5f)
	};

	polygon = std::make_shared<polygonline>(local_vertices, pos, scale, rotation);

	f32 x_offset = 10.0f;
	f32 y_offset = 20.0f;

	for (size_t i = 0; i < LIVES; i++)
	{
		lives.push_back(
		std::make_shared<polygonline>(local_vertices, glm::vec2(x_offset, height - y_offset), 10.0f, 0.0f)
		);

		x_offset += 12.0f;
	}
}

space_ship::~space_ship()
{
}

void space_ship::update(f32 rotation_speed)
{
	if (!is_respawining)
	{

		if (shield_on)
		{
			shield_time += Time::m_deltaTime;
			if (shield_time > SHIELDTIME)
			{
				shield_on = false;
				shield_time = 0.0f;
			}
		}

		// rotation
		if (Input::isKeyPressed(Key::Right))
			polygon->setangle(polygon->getangle() - rotation_speed * Time::m_deltaTime);

		if (Input::isKeyPressed(Key::Left))
			polygon->setangle(polygon->getangle() + rotation_speed * Time::m_deltaTime);

		// thrust
		if (Input::isKeyPressed(Key::Up))
		{
			move_force = MOVE_FORCE;
            
			particel_properties props;
			props.startColor = /*RGB_COLOR(161, 160, 257)*/color{0.0f, 0.0f, 0.8f, 1.0f};
			props.endColor = color{0.5f, 0.5f, 0.5f, 1.0f};
			props.startScale = glm::vec2(3.0f);
			props.endScale = glm::vec2(8.0f);
			props.lifeTime = 0.5f;
			props.position = polygon->getpos() - dir * 15.0f;
			props.scaleVariation = glm::vec2(0.0f);
			props.velocity = glm::vec2(0.0f, dir.y * -50.0f);
			props.velocityVariation = glm::vec2(50.0f);
			props.type = particleType::Circle;

			smoke.push(props, 1);
		}
		else
			move_force = 0.0f;

		// calculate ship direction
		dir = math::get_vector(1.0f, -polygon->getangle());

		// add bullet
		if (Input::isKeyDown(Key::Space))
		{
			Bullets.push_back(
				std::make_shared<Bullet>
				(polygon->getpos() + dir * polygon->getraduis(), dir, BULLET_SPEED + move_force * Time::m_deltaTime)
			);
		}

		// update ship position
		acceleration += move_force * dir;
		velocity += acceleration * Time::m_deltaTime;
		if (velocity.x != 0.0f || velocity.y != 0.0f)
			polygon->move(velocity * Time::m_deltaTime);
		velocity *= drag;
		acceleration = glm::vec2(0.0f);

		// clamp psoition in window size
		f32 width = window::getInstance()->getWidth();
		f32 height = window::getInstance()->getHeight();
		if (polygon->getpos().x < 0.0f)
			polygon->setpos_x(width);
		if (polygon->getpos().x > width)
			polygon->setpos_x(0.0f);
		if (polygon->getpos().y < 0.0f)
			polygon->setpos_y(height);
		if (polygon->getpos().y > height)
			polygon->setpos_y(0.0f);

		polygon->update();
	}
	else
	{
		respawn_time += Time::m_deltaTime;
		if (respawn_time > RESPAWNTIME)
		{
			f32 width = window::getInstance()->getWidth();
			f32 height = window::getInstance()->getHeight();
			is_respawining = false;
			respawn_time = 0.0f;
			polygon->setpos(glm::vec2(width * 0.5f, height * 0.5f));
			polygon->setangle(0.0f);
			polygon->update();
			velocity = glm::vec2(0.0f);
			fade = 1.0f;
			shield_on = true;
		}
	}

	// remove bullets out of range
	for (size_t i = 0; i < Bullets.size(); i++)
	{
		if (Bullets[i]->pos.x < 0.0f || Bullets[i]->pos.x > window::getInstance()->getWidth() ||
			Bullets[i]->pos.y < 0.0f || Bullets[i]->pos.y > window::getInstance()->getHeight())
		{
			Bullets.erase(Bullets.begin() + i);
		}
	}

	// update bullets position
	for (size_t i = 0; i < Bullets.size(); i++)
		Bullets[i]->pos += Bullets[i]->dir;
}

void space_ship::render(const color& ship_color, const color& bullet_color)
{
	smoke.update();

	if (shield_on)
		polygon->render(GREEN, 1.0f);
	else
	{
		if (!is_respawining)
			polygon->render(BLUE, 1.0f);
		else
		{
			fade = math::lerp(fade, 0.0f, Time::m_deltaTime);
			polygon->render(color{ 0.0f, 0.0f, 1.0f, fade }, 1.0f);
		}
	}
	for (size_t i = 0; i < Bullets.size(); i++)
		renderer::render_quad(Bullets[i]->pos, glm::vec2(5.0f), bullet_color);

	for (size_t i = 0; i < lives.size(); i++)
		lives[i]->render(GREEN, 0.5f);
	
	smoke.render();
}

void space_ship::Kill()
{
	if (!shield_on)
	{
		if (lives.size() > 0)
		{
			lives.pop_back();
			is_respawining = true;
		}
		else
			GameStateManger.set_state(new main_menu());
	}
}