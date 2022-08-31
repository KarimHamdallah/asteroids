#include "main_menu.h"
#include <snap_engine.h>
#include "asteroid.h"

void main_menu::init()
{
	Input::show_cursor(false);
	gameFont = fonts::load_font("assets/fonts/BadMofo.ttf", 48);
}

void main_menu::update()
{
	if (Input::isButtonPressed(MouseButton::MouseButtonLeft))
	{
		particel_properties props;
		props.startColor = RGB_COLOR(161, 160, 157);
		props.endColor = WHITE;
		props.startScale = glm::vec2(5.0f);
		props.endScale = glm::vec2(1.0f);
		props.lifeTime = 0.5f;
		props.position = Input::getMousePos();
		props.scaleVariation = glm::vec2(50.0f);
		props.velocity = glm::vec2(0.0f, -100.0f);
		props.velocityVariation = glm::vec2(500.0f);
		props.type = particleType::Circle;

		vfx.push(props, 10);
	}
	vfx.update();


	if (Input::isKeyDown(Space))
		GameStateManger.set_state(new game_play());

	if (Input::isKeyDown(Key::EscapeKey))
		window::getInstance()->setShouldClose(true);
}

void main_menu::render()
{
	renderer::render_text(&gameFont, "press     space    to    start", glm::vec2(170, 290), glm::vec2(1.0f, 0.5f), GREEN);
	renderer::render_text(&gameFont, "press     escape   to     quit", glm::vec2(170, 240), glm::vec2(1.0f, 0.5f), RED);
	renderer::render_text(&gameFont, "ASTEROIDS", glm::vec2(327, 412), glm::vec2(1.0f), WHITE);
	renderer::render_text(&gameFont, "2022 KRAZY technologies", glm::vec2(300.0f, 50.0f) , glm::vec2(0.5f), WHITE);
	renderer::render_quad(Input::getMousePos(), glm::vec2(10.0f), WHITE);
	vfx.render();
}

void main_menu::shutdown()
{
	LOG_CRITICAL("main_menu shutdown");
}