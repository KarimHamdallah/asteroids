#include "asteroid.h"

int main(int argc, char* argv[])
{
	{
		asteroid* game = asteroid::getInstance();
		game->run();

		delete game;
	}
	return 0;
}