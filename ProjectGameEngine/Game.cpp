#include "Game.h"



Game::Game(int argc, char ** argv) {
	// use arguments for some settings maybe
}

bool Game::init() {
	// TODO: move the following to a lower level
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
	{
		printf("SDL_Init error: %s", SDL_GetError());
		return false;
	}

	// initialise variables
	if ((window = new Window(windowTitle, windowWidth, windowHeight)) == nullptr) {
		return false;
	}

	renderer = new RenderBox(window);
	if (!renderer->init()) {
		return false;
	}

	// init audio
	AudioBox::get().init(6, 44100, 1024);


	// test variables
	bg = new Surface("textures\\bg.bmp");
	bird = new Surface("textures\\bird.PNG");
	sound = AudioBox::loadSound("sound\\fanfare.wav");


	// init timers
	timer = new Timer();
	updateTimer = new UpdateTimer(timer, MS_PER_UPDATE);


	return true;
}


void Game::cleanup() {
	delete
		renderer,
		window,
		timer,
		updateTimer,
		sound;

	delete bg, bird; // test vars

	// TODO: move the following to a lower level
	SDL_Quit();
}


void Game::run() {
	timer->start();

	/*
	start game loop

	processing input and rendering are flexible
	update is bound by MS_PER_UPDATE

	TODO: should be possible to have an upper limit on number of times
	per second to run render() so we don't have higher fps than the monitor can handle
	for example 300 fps would be wasteful on a 60 hertz monitor
	*/
	while (running) {
		timer->update();

		processEvents();

		// ensure game state updates at a constant rate, unaffected by the speed of the game loop
		while (updateTimer->isTimeToUpdate()) {
			update();
			updateTimer->updated();
		}

		render();
	}
}


void Game::processEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			running = false; break;
		case SDL_KEYDOWN:
			processInput(e);
		default: break;
		}
	}
}



void Game::processInput(const SDL_Event& e) {
	switch (e.key.keysym.sym) {
	case SDLK_LEFT:
		birdPos.adjustX(-4);
		break;
	case SDLK_RIGHT:
		birdPos.adjustX(4);
		break;
	case SDLK_UP:
		birdPos.adjustY(-4);
		break;
	case SDLK_DOWN:
		birdPos.adjustY(4);
		break;
	default:break;
	}
}

void Game::update() {
	// TODO: game logic

}

void Game::render() {
	// TODO: render stuff

	renderer->blit(bg);
	renderer->blit(bird, &birdPos);
	renderer->update();
}
