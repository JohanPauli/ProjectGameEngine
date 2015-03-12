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
#include "Physics.h"
#include "World.h"
#include "EntityGenerators.h"


const char* Game::WINDOW_TITLE = "Flappy Bird Demo";


Game::Game(int argc, char ** argv) 
: _window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT) {
	// use arguments for some settings maybe

	// init timers
	_timer = new Timer();
	_logicUpdateTimer = UpdateTimer(_timer, MS_PER_UPDATE);
	_renderUpdateTimer = UpdateTimer(_timer, MS_PER_RENDER);

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

	// init textures

	auto renderer = _window.getRenderer();

	RessourceManager &rManager = RessourceManager::getInstance();
	rManager.load("loadDocument.txt", renderer);
	

	// create entities
	auto botBorder = new Entity(bot);
	auto topBorder = new Entity(top);

	EntityGenerator::getInstance().init(_window.getWidth(), _window.getHeight(), _timer);

	auto player = EntityGenerator::getInstance().generatePlayerBird();
	auto pipes = EntityGenerator::getInstance().generatePipes(100, 100, 200);
	auto pipes2 = EntityGenerator::getInstance().generatePipes(500, 500, 200);
	auto bg = EntityGenerator::getInstance().generateBackground();
	auto fg = EntityGenerator::getInstance().generateForeground();
	_world->addEntity(bg, EntityType::BACKGROUND);
	_world->addEntity(fg, EntityType::FOREGROUND);

	_world->setPlayer(player);
	_world->addEntity(pipes.first, EntityType::STATIC);
	_world->addEntity(pipes.second, EntityType::STATIC);
	_world->addEntity(pipes2.first, EntityType::STATIC);
	_world->addEntity(pipes2.second, EntityType::STATIC);
	_world->setBorders(topBorder, botBorder);

	Level level(_window.getWidth(), _window.getHeight());

	_world->init(level);

	// activate all static entities
	while (_world->activateLeftEntity(EntityType::STATIC));
	while (_world->activateRightEntity(EntityType::STATIC));
	while (_world->activateLeftEntity(EntityType::BACKGROUND));
	while (_world->activateRightEntity(EntityType::BACKGROUND));
	while (_world->activateLeftEntity(EntityType::FOREGROUND));
	while (_world->activateRightEntity(EntityType::FOREGROUND));

	return true;
}


void Game::cleanup() {
	delete _world;

	// shut down low-level modules
	Flappy::quit();
}


void Game::run() {
	_timer->start();

	// main game loop
	while (_running) {
		_timer->update(); // update the game's timers

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