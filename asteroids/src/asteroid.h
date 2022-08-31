#include <engine/application/application.h>
#include "game_state.h"

class asteroid : public application
{
public:
	asteroid();
	~asteroid();

	void start();
	void update();
	void render();
	void run();

	inline static asteroid* getInstance() { if (!instance) { instance = new asteroid(); } return instance; }

public:
	state_manger m_GameStateManger;
	glm::vec2 m_windowCenter;
private:
	static asteroid* instance;
};

#define GameStateManger asteroid::getInstance()->m_GameStateManger
#define WindowCenter asteroid::getInstance()->m_windowCenter