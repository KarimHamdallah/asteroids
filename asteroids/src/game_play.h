#pragma once
#include "game_state.h"
#include "entities/space_ship.h"
#include "entities/star.h"
#include "systems/particle_system.h"
#include "entities/alien.h"
#include <memory>
#include <engine/renderer/font.h>

class game_play : public game_state
{
public:
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void shutdown();
	~game_play();

private:
	std::shared_ptr<space_ship> player;
	std::vector<std::shared_ptr<star>> stars;
	color ship_color;
	color bullet_color;
	particle_system vfx;

	std::shared_ptr<alien> Alien;
	font gameFont;

	f32 score;

	void destroy_star(u32 index);
	void player_vs_asteroids();
	void player_vs_alien();
	void bullets_vs_alien();
	void player_vs_bullets();
};