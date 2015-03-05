#include "Game.h"

#include "Timer.h"
#include "Audio.h"
#include "Rendering.h"
#include "Physics.h"
#include "Entities.h"
#include "Sprites.h"
#include "InputMapping.h"
#include "InputPolling.h"


Game::Game(int argc, char ** argv) {
	// use arguments for some settings maybe
}

bool Game::init() {
	
	// initialise variables
	if ((_window = new Window(_windowTitle, _windowWidth, _windowHeight)) == nullptr)
		return false;
	

	// init audio
	Audio::get().init(6, 44100, 1024);

	// init timers
	_timer = new Timer();
	_updateTimer = new UpdateTimer(_timer, MS_PER_UPDATE);

	_inputMapper = new InputMapper();

	// test variables
	phyEng = new PhysicsEngine();
	auto playerPhy = new DynamicPhysics(0.f, 0.1f, 0.2f, 0.f, 0.f, 100.f, 60.f, 90.f);
	auto pipePhy = new StaticPhysics(0.f, 0.f, 0.f, 0.f, 200.f, 0.f, 240.f, 130.f);
	phyEng->addDynamicPhysics(playerPhy);
	phyEng->addStaticPhysics(pipePhy);

	auto renderer = _window->getRenderer();
	bird = renderer->loadSprite("assets\\sprites\\bird_ani_sheet.png");
	pipeTop = renderer->loadSprite("assets\\sprites\\pipe-top.png");
	pipeMid = renderer->loadSprite("assets\\sprites\\pipe-mid.png");
	pipeBot = renderer->loadSprite("assets\\sprites\\pipe-bot.png");

	player = new PlayerEntity(playerPhy, bird);
	pipe = new PipeEntity(pipePhy, pipeBot, pipeMid, pipeTop);
	_inputMapper->registerContext(player);
	_inputMapper->activateContext(InputContextType::BIRD);

	//sound = AudioBox::get().loadSound("sound\\fanfare.wav");


	return true;
}


void Game::cleanup() {
	delete
		_window,
		_timer,
		_updateTimer,
		_inputMapper;
		

	delete bird, sound, phyEng, player, pipeTop, pipeMid, pipeBot; // test vars

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

		// handle user input
		InputHandling::pollInput(_inputMapper, _running);

		// ensure game state updates at a constant rate, unaffected by the speed of the game loop
		while (_updateTimer->isTimeToUpdate()) {
			update();
			_updateTimer->updated();
		}

		render();
	}
}


void Game::update() {
	// TODO: game logic
	phyEng->update();
	player->update();
}

void Game::render() {
	// TODO: render stuff

	auto renderer = _window->getRenderer();

	player->render(renderer);
	pipe->render(renderer);;
	_window->update();
}
