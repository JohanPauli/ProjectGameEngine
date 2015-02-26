#ifndef __GAME_H__
#define __GAME_H__

#include <SDL.h>

#include "Timer.h"
#include "AudioBox.h"
#include "Window.h"
#include "RenderBox.h"

class Game {
private:
	const static long MS_PER_UPDATE = 8;	// how many milliseconds should be between each call of update()

	Timer*		 timer = nullptr;		// general timer for the game
	UpdateTimer* updateTimer = nullptr;	// game logic timer
	Window*		 window = nullptr;
	RenderBox*	 renderer = nullptr;



	// settings
	char* windowTitle = "Window Title";
	int   windowWidth = 1024;
	int   windowHeight = 768;
	bool  running = true;


	//// test stuff
	Surface* bg = nullptr;
	Surface* bird = nullptr;
	Rect	 birdPos = Rect(0, 0, 64, 64);
	Sound*	 sound = nullptr;

private:
	// release resources and stuff game ends
	void cleanup();

	void processEvents();

	// handle any user input
	void processInput(const SDL_Event& e);	// SDL_Event needs blackboxing

	// run game logic
	void update();

	// render the graphics
	void render();

public:
	Game(int argc, char ** argv);

	bool init();

	// start the game loop
	void run();

};



#endif