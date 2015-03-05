#pragma once


#include <SDL.h>

// forward declarations
class Timer;
class UpdateTimer;
class Window;
class PhysicsEngine;
class PlayerEntity;
class PipeEntity;
class Sprite;
class Sound;
class Music;
class InputMapper;



class Game {
private:
	const static long MS_PER_UPDATE = 8;	// how many milliseconds should be between each call of update()

	Timer*		 _timer = nullptr;			// general timer for the game
	UpdateTimer* _updateTimer = nullptr;	// game logic timer
	Window*		 _window = nullptr;
	InputMapper* _inputMapper = nullptr;


	// settings
	char* _windowTitle = "Window Title";
	int   _windowWidth = 1024;
	int   _windowHeight = 768;
	bool  _running = true;


	//// test stuff
	PhysicsEngine*	phyEng = nullptr;
	PlayerEntity*	player = nullptr;
	PipeEntity*		pipe = nullptr;

	Sprite*	bird = nullptr;
	Sprite*	pipeTop = nullptr;
	Sprite*	pipeMid = nullptr;
	Sprite*	pipeBot = nullptr;
	Sound*	sound = nullptr;

private:
	// release resources and stuff game ends
	void cleanup();

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