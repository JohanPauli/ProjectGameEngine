#include "Game.h"

#include "Audio.h"
#include "Sprites.h"
#include "Misc.h"
#include "Rect.h"
#include "Entity.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"
#include "InputMapping.h"
#include "RessourceManager.h"


const char* Game::WINDOW_TITLE = "Flappy Bird Demo";


Game::Game(int argc, char ** argv) 
: _window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT) {
	// use arguments for some settings maybe

	// init timers
	_timer = Timer();
	_logicUpdateTimer = UpdateTimer(&_timer, MS_PER_UPDATE);
	_renderUpdateTimer = UpdateTimer(&_timer, MS_PER_RENDER);

	// init audio
	Audio::get().init(6, 44100, 1024);
}

Game::~Game() {
	cleanup();
}

bool Game::init() {
	_world = new World(_window.getWidth(), _window.getHeight());
	// test variables
	// physics
	auto top = new StaticPhysics(0.f, 0.f, 0.f, 0.f,
		-100.f, -100.f,		// x, y
		 100.f, 10000.f);	// height, width
	auto bot = new StaticPhysics(0.f, 0.f, 0.f, 0.f,
		-100.f, (float)_window.getHeight(), // x, y
		10000.f, 10000.f);			  // height, width

	auto playerPhy = new DynamicPhysics(0.f, 0.08f, 
										2.f, 0.f, 
										0.f, 100.f, 
										60.f, 90.f);
	auto pipePhy = new StaticPhysics(0.f, 0.f, 0.f, 0.f, 
									 250.f, _window.getHeight()-240.f, // x, y
									 240.f, 130.f); // h, w
	auto pipePhy2 = new StaticPhysics(0.f, 0.f, 0.f, 0.f,
									  250.f, 0.f, // x, y
									  240.f, 130.f); // h, w

	// init textures

	auto renderer = _window.getRenderer();

	RessourceManager &rManager = RessourceManager::getInstance(renderer);
	rManager.load("loadDocument.txt");
	

	auto bird = rManager.getByTag<Sprite*>("bird");
	auto pipeMid = rManager.getByTag<Sprite*>("pipemid");
	auto pipeBot = rManager.getByTag<Sprite*>("pipebot");

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

	// register
	InputMapper::getInstance().registerContext(input, 10);


	// create entities
	auto botBorder = new Entity(bot);
	auto topBorder = new Entity(top);


	auto player = new Entity(playerPhy, birdGraphics, input);
	auto pipe = new Entity(pipePhy, pipeGraphics);
	auto pipe2 = new Entity(pipePhy2, pipeGraphics2);

	_world->setPlayer(player);
	_world->addEntity(pipe, EntityType::STATIC);
	_world->addEntity(pipe2, EntityType::STATIC);
	_world->setBorders(topBorder, botBorder);


	// activate all static entities
	while (_world->activateLeftEntity(EntityType::STATIC));
	while (_world->activateRightEntity(EntityType::STATIC));

	return true;
}


void Game::cleanup() {
	delete _world;

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
	InputMapper::getInstance().notify();
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
	_world->update();
}


void Game::render() {
	_world->render(_window.getRenderer());
	_window.update();
}
