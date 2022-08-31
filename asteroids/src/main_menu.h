#pragma once
#include "game_play.h"
#include <engine/defines.h>
#include "systems/particle_system.h"
#include <engine/renderer/font.h>

class main_menu : public game_state
{
public:
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void shutdown();

private:
	particle_system vfx;
	font gameFont;
};