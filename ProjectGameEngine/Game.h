#ifndef __GAME_H__
#define __GAME_H__

#include <SDL.h>

#include "Timer.h"
#include "AudioBox.h"
#include "Rendering.h"
#include "Physics.h"
#include "Entity.h"

class Game {
private:
	const static long MS_PER_UPDATE = 8;	// how many milliseconds should be between each call of update()

	Timer*		 _timer = nullptr;		// general timer for the game
	UpdateTimer* _updateTimer = nullptr;	// game logic timer
	Window*		 _window = nullptr;



	// settings
	char* _windowTitle = "Window Title";
	int   _windowWidth = 1024;
	int   _windowHeight = 768;
	bool  _running = true;


	//// test stuff
	PhysicsEngine*	phyEng = nullptr;
	Player*			player = nullptr;
	Texture*		bg = nullptr;

	Texture* bird = nullptr;
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