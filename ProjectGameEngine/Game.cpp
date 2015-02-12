#include "Game.h"

#include <chrono>


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

	audio = new AudioBox();
	if (!audio->init()) {
		return false;
	}

	// test variables
	bg = new Surface("textures\\bg.bmp");
	bird1 = new Surface("textures\\bird.bmp");

	return true;
}


void Game::cleanup() {
	delete
		audio,
		renderer,
		window;

	delete bg, bird1; // test vars

	// TODO: move the following to a lower level
	SDL_Quit();
}


void Game::run() {
	timer.start();

	/*
	start game loop

	processing input and rendering are flexible
	update is bound by MS_PER_UPDATE

	TODO: should be possible to have an upper limit on number of times
	per second to run render() so we don't have higher fps than the monitor can handle
	for example 300 fps would be wasteful on a 60 hertz monitor
	*/
	while (running) {
		timer.update();

		processEvents();

		// ensure game state updates at a constant rate, unaffected by the speed of the game loop
		while (timer.getUpdateLag() >= MS_PER_UPDATE) {
			update();
			timer.trimLag(MS_PER_UPDATE);
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
		*bird1Pos.x() -= 4;
		break;
	case SDLK_RIGHT:
		*bird1Pos.x() += 4;
		break;
	case SDLK_UP:
		*bird1Pos.y() -= 4;
		break;
	case SDLK_DOWN:
		*bird1Pos.y() += 4;
		break;
	default:break;
	}
}

void Game::update() {
	// TODO: game logic

	renderer->blit(bg);
	renderer->blit(bird1, &bird1Pos);
	renderer->update();
}

void Game::render() {
	// TODO: render stuff

}


// time since epoch in milliseconds
long long Game::currentTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch())
		.count();
}