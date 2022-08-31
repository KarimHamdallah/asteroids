#pragma once
#include "space_ship.h"


class alien
{
public:
	alien(const glm::vec2& pos, f32 scale);
	~alien();
	void update(const glm::vec2& player_pos);
	void render(const color& alien_color, const color& bullet_color);

	std::shared_ptr<polygonline> polygon;
	std::vector<std::shared_ptr<Bullet>> Bullets;
	f32 rotation;
	bool isActive;
	f32 spawnTime;
	glm::vec2 dir;
	f32 changeDirectionTimer;
	f32 shootTimer;
private:
	particle_system smoke;
	

	void move();
	void shoot(const glm::vec2& player_pos);
	void update_bullets();
};