#include "asteroid.h"
#include "game_play.h"
#include "main_menu.h"

asteroid* asteroid::instance;

asteroid::asteroid()
	: application()
{
	m_windowCenter = glm::vec2(win->getWidth() * 0.5f, win->getHeight() * 0.5f);
}

asteroid::~asteroid()
{
}

void asteroid::start()
{
	// set state and init
	m_GameStateManger.set_state(new main_menu());
}

void asteroid::update()
{
	m_GameStateManger.update();
}

void asteroid::render()
{
	m_GameStateManger.render();
}

void asteroid::run()
{
	start();

	while (!win->shouldClose())
	{
		Time::time_update();
		win->update();

		update();

		renderer::renderer_begin();
		render();
		renderer::renderer_end(win->getRenderingWindow());
		Time::time_update_late();
	}
}