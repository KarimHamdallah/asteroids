#include "game_play.h"
#include <snap_engine.h>
#include "asteroid.h"
#include "main_menu.h"

#define ASTEROIDS_COUNT 3

void game_play::init()
{
	// hide cursor
	Input::show_cursor(false);
	// creat player
	player = std::make_shared<space_ship>(WindowCenter, 20.0f, 0.0f);
	ship_color = BLUE;
	bullet_color = BLUE;

	// window dimensions
	f32 width = window::getInstance()->getWidth();
	f32 height = window::getInstance()->getHeight();

	glm::vec2 corners[]{ 
		glm::vec2(0.0f, height),
		glm::vec2(width, height),
		glm::vec2(width, 0.0f),
		glm::vec2(0.0f, 0.0f)
	};
	// reserve asteroids count
	stars.reserve(ASTEROIDS_COUNT);
	for (size_t i = 0; i < ASTEROIDS_COUNT; i++)
		stars.emplace_back(std::make_shared<star>(
			glm::vec2(random(-1.0f, 5.0f), random(-1.0f, 5.0f)), random(50.0f, 70.0f), random(0.0f, 360.0f) )
		);

	Alien = std::make_shared<alien>(WindowCenter, 20.0f);

	gameFont = fonts::load_font("assets/fonts/BadMofo.ttf", 48);
	score = 0.0f;
}

void game_play::update()
{
	// update player
	player->update(200.0f);

	// update stars
	for (size_t i = 0; i < stars.size(); i++)
		stars[i]->update();
	
	// collsions
	for (size_t i = 0; i < player->Bullets.size(); i++)
	{
		auto bullet = player->Bullets[i];
		for (size_t j = 0; j < stars.size(); j++)
		{
			auto star = stars[j];

			// outer collsion
			if (collisions::point_vs_circle(bullet->pos, star->position, star->scale.x))
			{
				// inner collison
				if (collisions::point_vs_polygon(bullet->pos, star->transformed_vertices))
				{
					// remove bullet
					player->Bullets.erase(player->Bullets.begin() + i);
					// remove star
					destroy_star(j);
					break;
				}
			}
		}
	}

	if (!player->is_respawining)
	{
		player_vs_asteroids();
		if (Alien->isActive)
		{
			player_vs_alien();
			bullets_vs_alien();
		}
		player_vs_bullets();
	}

	vfx.update();
	Alien->update(player->polygon->getpos());

	if (Input::isKeyDown(Key::EscapeKey))
		GameStateManger.set_state(new main_menu());
}

void game_play::render()
{
	player->render(ship_color, bullet_color);

	for (size_t i = 0; i < stars.size(); i++)
		stars[i]->render();

	vfx.render();
	Alien->render(RED, RED);

	f32 height = WindowCenter.y * 2.0f;
	renderer::render_text(&gameFont, "Score : " + std::to_string(score), glm::vec2(10.0f, height - 50.0f),
		glm::vec2(0.5f), WHITE);

	renderer::render_text(&gameFont, "press escape to return : ", glm::vec2(10.0f, height - 100.0f),
		glm::vec2(0.5f), RED);
}

void game_play::shutdown()
{
	LOG_CRITICAL("game_play shutdown");
	// remove stars and free memory
	stars.clear();
	stars.shrink_to_fit();
}

game_play::~game_play()
{
}

void game_play::destroy_star(u32 index)
{
	glm::vec2 pos = stars[index]->position;
	f32 scale = stars[index]->scale.x;

	particel_properties props;
	props.startColor = RGB_COLOR(161, 160, 157);
	props.endColor = WHITE;
	props.startScale = glm::vec2(10.0f);
	props.endScale = glm::vec2(1.0f);
	props.lifeTime = 0.5f;
	props.position = stars[index]->position;
	props.scaleVariation = glm::vec2(50.0f);
	props.velocity = glm::vec2(0.0f, -1.0f);
	props.velocityVariation = glm::vec2(500.0f);

	
	// break into two
	if (scale > 40.0f)
	{
		stars.push_back(std::make_shared<star>
			(pos, scale - 10.0f, random(0.0f, 360.0f))
		);

		stars.push_back(std::make_shared<star>
			(pos, scale - 10.0f, random(0.0f, 360.0f))
		);

		props.type = particleType::AABB;
		vfx.push(props, 20);
	}
	else
	{
		score += scale;
		props.type = particleType::Circle;
		vfx.push(props, 20);
	}

	stars.erase(stars.begin() + index);

	// new wave
	if (stars.size() < 2)
	{
		for (size_t i = 0; i < ASTEROIDS_COUNT; i++)
			stars.push_back(std::make_shared<star>(
				glm::vec2(random(-1.0f, 5.0f), random(-1.0f, 5.0f)), random(50.0f, 70.0f), random(0.0f, 360.0f))
			);
	}
}

void game_play::player_vs_asteroids()
{
	for (size_t i = 0; i < stars.size(); i++)
	{
		if (collisions::circle_vs_circle(player->polygon->getpos(), player->polygon->getraduis() * 2.3f, stars[i]->position, stars[i]->scale.x))
		{
			if (collisions::polygon_vs_polygon(player->polygon->getTransformedVertices(), stars[i]->transformed_vertices))
			{
				player->Kill();
				// add particles
				if (!player->shield_on && player->is_respawining)
					vfx.push(blue_vfx(player->polygon->getpos()), 100);
				break;
			}
		}
	}
}

void game_play::player_vs_alien()
{
	if (collisions::circle_vs_circle(player->polygon->getpos(), player->polygon->getraduis() * 2.3f, Alien->polygon->getpos(), Alien->polygon->getraduis()))
	{
		if (collisions::polygon_vs_polygon(player->polygon->getTransformedVertices(), Alien->polygon->getTransformedVertices()))
		{
			player->Kill();
			// add particles
			if (!player->shield_on && player->is_respawining)
				vfx.push(blue_vfx(player->polygon->getpos()), 100);
		}
	}
}

void game_play::bullets_vs_alien()
{
	for (size_t i = 0; i < player->Bullets.size(); i++)
	{
		auto bullet = player->Bullets[i];

		// outer collsion
		if (collisions::point_vs_circle(bullet->pos, Alien->polygon->getpos(), Alien->polygon->getraduis()))
		{
			// inner collison
			if (collisions::point_vs_polygon(bullet->pos, Alien->polygon->getTransformedVertices()))
			{
				// remove bullet
				player->Bullets.erase(player->Bullets.begin() + i);
				// reset Alien
				Alien->isActive = false;
				Alien->spawnTime = random(40.0, 60.0f);

				// particle
				particel_properties props;
				props.startColor = RED;
				props.endColor = color{0.1f, 0.0f, 0.0f, 1.0f};
				props.startScale = glm::vec2(10.0f);
				props.endScale = glm::vec2(1.0f);
				props.lifeTime = 0.5f;
				props.position = Alien->polygon->getpos();
				props.scaleVariation = glm::vec2(50.0f);
				props.velocity = glm::vec2(0.0f, -1.0f);
				props.velocityVariation = glm::vec2(500.0f);
				props.type = particleType::Circle;
				vfx.push(props, 10.0f);

				// break
				break;
			}
		}
	}
}

void game_play::player_vs_bullets()
{
	for (size_t i = 0; i < Alien->Bullets.size(); i++)
	{
		auto bullet = Alien->Bullets[i];

		// outer collsion
		if (collisions::point_vs_circle(bullet->pos, player->polygon->getpos(), player->polygon->getraduis() * 2.3f))
		{
			// inner collison
			if (collisions::point_vs_polygon(bullet->pos, player->polygon->getTransformedVertices()))
			{
				// remove bullet
				Alien->Bullets.erase(Alien->Bullets.begin() + i);

				// kill player
				player->Kill();
				// add particles
				if (!player->shield_on && player->is_respawining)
					vfx.push(blue_vfx(player->polygon->getpos()), 100);

				// break
				break;
			}
		}
	}
}