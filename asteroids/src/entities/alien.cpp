#include "alien.h"
#include <snap_engine.h>

#define MOVE_SPEED 2.0f
#define CHANGE_DIR_TIME 2.0f
#define BULLET_SPEED 20.0f
#define SHOOT_TIME 1.0f


alien::alien(const glm::vec2 & pos, f32 scale)
{

	std::vector<glm::vec2> local_vertices =
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(-0.5f, -0.5f),
		glm::vec2(0.0f, -0.2f),
		glm::vec2(0.5f, -0.5f)
	};
	polygon = std::make_shared<polygonline>(local_vertices, pos, scale, -90.0f);

	rotation = 0.0f;
	isActive = false;
	spawnTime = random(10.0, 20.0f);
	changeDirectionTimer = CHANGE_DIR_TIME;
	dir = glm::vec2(1.0f, 0.0f);
	shootTimer = SHOOT_TIME;
}

alien::~alien()
{
}

void alien::update(const glm::vec2& player_pos)
{
	if (isActive)
	{
		move();
		shoot(player_pos);

		if (polygon->getpos().x > window::getInstance()->getWidth())
		{
			isActive = false;
			spawnTime = random(40.0, 60.0f);
		}

		polygon->update();
	}
	else
	{
		spawnTime -= Time::m_deltaTime;
		if (spawnTime <= 0)
		{
			isActive = true;
			polygon->setpos(glm::vec2(0.0f,
				random(100.0f, window::getInstance()->getHeight() - 100.0f)
			));
			polygon->update();
		}
	}

	update_bullets();
}

void alien::render(const color & alien_color, const color & bullet_color)
{
	if (isActive)
	{
		polygon->render(alien_color, 1.0f);
	}

	for (size_t i = 0; i < Bullets.size(); i++)
		renderer::render_quad(Bullets[i]->pos, glm::vec2(5.0f), RED);
}

void alien::move()
{
	polygon->move(dir * MOVE_SPEED);

	if (polygon->getpos().y > window::getInstance()->getHeight())
		polygon->setpos(glm::vec2(polygon->getpos().x, 0.0f));
	if (polygon->getpos().y < 0.0f)
		polygon->setpos(glm::vec2(polygon->getpos().x, window::getInstance()->getHeight()));

	changeDirectionTimer -= Time::m_deltaTime;

	if (changeDirectionTimer <= 0.0f)
	{
		changeDirectionTimer = CHANGE_DIR_TIME;
		//dir = glm::vec2(random(0.0f, 1.0f), random(-1.0f, 1.0f));
		f32 angle = random(-150.0f, -50.0f);
		dir = math::get_vector(1.0f, -angle);

		polygon->setangle(angle);
	}
}

void alien::shoot(const glm::vec2& player_pos)
{
	if (shootTimer <= 0)
	{
		glm::vec2 direction = player_pos - polygon->getpos();
		direction = glm::normalize(direction);
		Bullets.push_back(std::make_shared<Bullet>(dir * 10.0f + polygon->getpos(), direction, BULLET_SPEED * Time::m_deltaTime));

		shootTimer = SHOOT_TIME;
	}
	else
	{
		shootTimer -= Time::m_deltaTime;
	}
}

void alien::update_bullets()
{
	for (size_t i = 0; i < Bullets.size(); i++)
	{
		if (Bullets[i]->pos.x < 0.0f ||
			Bullets[i]->pos.x > window::getInstance()->getWidth() ||
			Bullets[i]->pos.y < 0.0f ||
			Bullets[i]->pos.y > window::getInstance()->getHeight())
		{
			Bullets.erase(Bullets.begin() + i);
		}
	}

	for (size_t i = 0; i < Bullets.size(); i++)
		Bullets[i]->pos += Bullets[i]->dir * BULLET_SPEED;
}