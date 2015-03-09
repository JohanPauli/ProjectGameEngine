#include "Game.h"

#include "Audio.h"
#include "Sprites.h"
#include "Misc.h"
#include "Rect.h"
#include "Entity.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"


const char* Game::WINDOW_TITLE = "Flappy Bird Demo";



Game::Game(int argc, char ** argv) 
: _window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT) {
	// use arguments for some settings maybe

	_windowWidth = WINDOW_WIDTH;
	_windowHeight = WINDOW_HEIGHT;
}

Game::~Game() {
	cleanup();
}

bool Game::init() {
	// init audio
	Audio::get().init(6, 44100, 1024);

	// init timers
	_timer = Timer();
	_logicUpdateTimer = UpdateTimer(&_timer, MS_PER_UPDATE);
	_renderUpdateTimer = UpdateTimer(&_timer, MS_PER_RENDER);


	// test variables
	// physics
	auto playerPhy = new DynamicPhysics(0.f, 0.08f, 
										2.f, 0.f, 
										0.f, 100.f, 
										60.f, 90.f);
	auto pipePhy = new StaticPhysics(0.f, 0.f, 
									 0.f, 0.f, 
									 250.f, _windowHeight-240.f, 
									 240.f, 130.f);
	auto pipePhy2 = new StaticPhysics(0.f, 0.f,
									  0.f, 0.f,
									  250.f, 0.f,
									  240.f, 130.f);
	phyEng.addDynamicPhysics(playerPhy);
	phyEng.addStaticPhysics(pipePhy);
	phyEng.addStaticPhysics(pipePhy2);

	// init textures
	auto renderer = _window.getRenderer();
	bird = renderer->loadSprite("assets\\sprites\\bird_ani_sheet.png");
	pipeTop = renderer->loadSprite("assets\\sprites\\pipe-top.png");
	pipeMid = renderer->loadSprite("assets\\sprites\\pipe-mid.png");
	pipeBot = renderer->loadSprite("assets\\sprites\\pipe-bot.png");

	// create bird spritesheet
	std::vector<Rect> rector;
	rector.emplace_back(Rect(0, 0, 18, 12));
	rector.emplace_back(Rect(0, 0, 18, 12));
	rector.emplace_back(Rect(18, 0, 18, 12));
	rector.emplace_back(Rect(36, 0, 18, 12));
	rector.emplace_back(Rect(36, 0, 18, 12));
	rector.emplace_back(Rect(18, 0, 18, 12));
	auto birdSheet = new SpriteSheet(rector, bird);

	// components
	auto input = new PlayerInput();
	auto pipeGraphics = new PipeGraphics(pipeBot, pipeMid, true);
	auto pipeGraphics2 = new PipeGraphics(pipeBot, pipeMid, false);
	auto birdGraphics = new BirdGraphics(birdSheet, UpdateTimer(&_timer, 20));

	// activate player entities' input
	_inputMapper.registerContext(input, 10);
	_inputMapper.activateContext(input->getInputContextId());


	// create entities
	player = new Entity(playerPhy, birdGraphics, input);
	pipe = new Entity(pipePhy, birdGraphics);
	pipe2 = new Entity(pipePhy2, birdGraphics);


	return true;
}


void Game::cleanup() {

	// test vars
	delete bird, sound, phyEng,
		// entities
		player, pipe, pipe2,
		// textures
		pipeTop, pipeMid, pipeBot;

	// shut down low-level modules
	Flappy::quit();
}


void Game::run() {
	_timer.start();

	// main game loop
	while (_running) {
		_timer.update(); // update the game's timers

		handleUserInput();

		tryUpdate(); // updating is bound by MS_PER_UPDATE

		tryRender(); // rendering is bound by MS_PER_RENDER
	}
}


void Game::handleUserInput() {
	Flappy::pollEvents(_running);
	_inputMapper.notify();
}


void Game::tryUpdate() {
	// ensure game state updates at a constant rate, unaffected by the speed of the game loop
	while (_logicUpdateTimer.isTimeToUpdate()) {
		update();
		_logicUpdateTimer.updated();
	}
}


void Game::tryRender() {
	// lock maximum framerate
	if (_renderUpdateTimer.isTimeToUpdate()) {
		render();
		_renderUpdateTimer.catchUp();
	}
	else // release cpu resource if nothing to do
		Flappy::delay(1);
}


void Game::update() {

	pipe->update();
	pipe2->update();
	player->update();

	phyEng.detectColissions();
}


void Game::render() {

	auto renderer = _window.getRenderer();

	pipe->render(renderer);
	pipe2->render(renderer);
	player->render(renderer);

	_window.update();
}
