#include "Game.h"



Game::Game(int argc, char ** argv) {
	// use arguments for some settings maybe
}

bool Game::init() {
	
	// initialise variables
	if ((_window = new Window(_windowTitle, _windowWidth, _windowHeight)) == nullptr)
		return false;
	

	// init audio
	AudioBox::get().init(6, 44100, 1024);

	// init timers
	_timer = new Timer();
	_updateTimer = new UpdateTimer(_timer, MS_PER_UPDATE);



	// test variables
	auto renderer = _window->getRenderer();
	bg = renderer->loadTexture("textures\\bg.bmp");
	bird = renderer->loadTexture("textures\\bird.PNG");
	sound = AudioBox::get().loadSound("sound\\fanfare.wav");


	return true;
}


void Game::cleanup() {
	delete
		_window,
		_timer,
		_updateTimer;
		

	delete bg, bird, sound; // test vars

	// TODO: move SDL_Quit() to a lower level
	SDL_Quit();
}


void Game::run() {
	_timer->start();

	/*
	start game loop

	processing input and rendering are flexible
	update is bound by MS_PER_UPDATE

	TODO: should be possible to have an upper limit on number of times
	per second to run render() so we don't have higher fps than the monitor can handle
	for example 300 fps would be wasteful on a 60 hertz monitor
	*/
	while (_running) {
		_timer->update();

		processEvents();

		// ensure game state updates at a constant rate, unaffected by the speed of the game loop
		while (_updateTimer->isTimeToUpdate()) {
			update();
			_updateTimer->updated();
		}

		render();
	}
}


void Game::processEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			_running = false; break;
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
	Rect rect = Rect(0, 0, bg->getWidth(), bg->getHeight());

	auto renderer = _window->getRenderer();
	renderer->render(bg, &rect);
	renderer->render(bird, &birdPos);
	_window->update();
}
