#pragma once


#include <SDL.h>

#include "Timer.h"
#include "Rendering.h"
#include "InputMapping.h"
#include "Physics.h"

// forward declarations
class Entity;
class Sprite;
class Sound;
class Music;
class Graphics;



class Game {
private:
	const static long	MS_PER_UPDATE = 8;	// how many milliseconds should be between each call of update()
	const static long	MS_PER_RENDER = 8;	// milliseconds between each render

	// standard values
	const static int    WINDOW_WIDTH = 1024;
	const static int	WINDOW_HEIGHT = 768;
	const static char*	WINDOW_TITLE;

	// core
private:
	Window		_window;
	InputMapper _inputMapper;

	// timers
private:
	Timer		_timer;			// general timer for the game
	UpdateTimer _logicUpdateTimer;	// game logic timer
	UpdateTimer _renderUpdateTimer;

	// settings
private:
	bool _running = true;
	int	 _windowWidth;
	int	 _windowHeight;

	//// test variables
private:
	PhysicsEngine phyEng;
	Entity*	player = nullptr;
	Entity*	pipe = nullptr;
	Entity*	pipe2 = nullptr;

	Sprite*	bird = nullptr;
	Sprite*	pipeTop = nullptr;
	Sprite*	pipeMid = nullptr;
	Sprite*	pipeBot = nullptr;
	Sound*	sound = nullptr;

private:
	// release resources and stuff game ends
	void cleanup();

	// handle input
	void handleUserInput();

	// update logic at a frequency of MS_PER_UPDATE milliseconds
	void tryUpdate();

	// don't render more often than once every MS_PER_RENDER milliseconds
	void tryRender();

	// run game logic
	void update();

	// render the graphics
	void render();

public:
	Game(int argc, char ** argv);
	~Game();

	bool init();

	// start the game loop
	void run();

};