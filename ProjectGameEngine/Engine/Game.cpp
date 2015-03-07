#include "Game.h"

#include "Timer.h"
#include "Audio.h"
#include "Rendering.h"
#include "Physics.h"
#include "Entities.h"
#include "Sprites.h"
#include "InputMapping.h"
#include "Misc.h"


Game::Game(int argc, char ** argv) {
	// use arguments for some settings maybe
}

Game::~Game() {
	cleanup();
}

bool Game::init() {
	
	// initialise variables
	if ((_window = new Window(_windowTitle, _windowWidth, _windowHeight)) == nullptr)
		return false;
	

	// init audio
	Audio::get().init(6, 44100, 1024);

	// init timers
	_timer = new Timer();
	_logicUpdateTimer = new UpdateTimer(_timer, MS_PER_UPDATE);
	_renderUpdateTimer = new UpdateTimer(_timer, MS_PER_RENDER);

	_inputMapper = new InputMapper();

	// test variables
	// init physics
	phyEng = new PhysicsEngine();
	auto playerPhy = new DynamicPhysics(0.f, 0.08f, 
										2.f, 0.f, 
										0.f, 100.f, 
										60.f, 90.f);
	auto pipePhy = new StaticPhysics(0.f, 0.f, 
									 0.f, 0.f, 
									 200.f, _windowHeight-240.f, 
									 240.f, 130.f);
	auto pipePhy2 = new StaticPhysics(0.f, 0.f,
									  0.f, 0.f,
									  200.f, 0.f,
									  240.f, 130.f);
	phyEng->addDynamicPhysics(playerPhy);
	phyEng->addStaticPhysics(pipePhy);

	// init textures
	auto renderer = _window->getRenderer();
	bird = renderer->loadSprite("assets\\sprites\\bird_ani_sheet.png");
	pipeTop = renderer->loadSprite("assets\\sprites\\pipe-top.png");
	pipeMid = renderer->loadSprite("assets\\sprites\\pipe-mid.png");
	pipeBot = renderer->loadSprite("assets\\sprites\\pipe-bot.png");

	// create bird spritesheet
	std::vector<Rect> rector;
	rector.emplace_back(Rect(0, 0, 18, 12));
	rector.emplace_back(Rect(18, 0, 18, 12));
	rector.emplace_back(Rect(36, 0, 18, 12));
	auto birdSheet = new SpriteSheet(rector, bird);

	// create entities
	player = new PlayerEntity(playerPhy, birdSheet);
	pipe = new PipeEntity(pipePhy, pipeBot, pipeMid);
	pipe2 = new PipeEntity(pipePhy2, pipeBot, pipeMid, false);
	
	// activate player entities' input
	_inputMapper->registerContext(player, 10);
	_inputMapper->activateContext(player->getInputContextId());


	return true;
}


void Game::cleanup() {
	delete
		_window,
		_timer,
		_logicUpdateTimer,
		_inputMapper;
		
	// test vars
	delete bird, sound, phyEng,

		// entities
		player, pipe, pipe2, 

		// textures
		pipeTop, pipeMid, pipeBot; 

	// TODO: move SDL_Quit() to a lower level
	SDL_Quit();
}


void Game::run() {
	_timer->start();

	/*
	start game loop

	processing input and rendering are flexible
	update is bound by MS_PER_UPDATE
	render is bound by MS_PER_RENDER

	*/
	while (_running) {
		_timer->update();

		// handle input
		Flappy::pollEvents(_running);
		_inputMapper->notify();

		// ensure game state updates at a constant rate, unaffected by the speed of the game loop
		while (_logicUpdateTimer->isTimeToUpdate()) {
			update();
			_logicUpdateTimer->updated();
		}

		// render if enough time has passed
		if (_renderUpdateTimer->isTimeToUpdate()) {
			render();
			_renderUpdateTimer->catchUp();
		}
		// release cpu resource if nothing to do
		else
			Flappy::delay(1);
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
	pipe->render(renderer);
	pipe2->render(renderer);
	_window->update();
}
