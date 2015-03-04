#include "Game.h"


int main(int argc, char ** argv) {
	auto game = new Game(argc, argv);

	if (game->init())
		game->run();

	delete game;
	return 0;
}