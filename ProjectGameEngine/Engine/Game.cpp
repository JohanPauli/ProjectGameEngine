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
#include "Counter.h"
#include "Events.h"


const char* Game::WINDOW_TITLE = "Flappy Bird Demo";


Game::Game(int argc, char ** argv) 
: _window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT) {
	// use arguments for some settings maybe

	// init timers
	_timer = new Timer();
	_renderTimer = Timer();
	_logicUpdateTimer = UpdateTimer(_timer, MS_PER_UPDATE);
	_renderUpdateTimer = UpdateTimer(&_renderTimer, MS_PER_RENDER);

	// init singletons
	Audio::get().init(6, 44100, 1024);
	EntityGenerator::getInstance().init(_window.getWidth(), _window.getHeight(), _timer);
	RessourceManager::getInstance();

	// load resources
	RessourceManager::getInstance().load("loadDocument.txt", _window.getRenderer());
}

Game::~Game() {
	cleanup();
}

bool Game::init() {
	_timer->reset();
	// initialize game world
	Level level(_window.getWidth(), _window.getHeight());
	_world = new World(_window.getWidth(), _window.getHeight());
	_world->init(level);

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
	_renderTimer.start();

	// main game loop
	while (_running) {
		// update the logic and rendering timers
		_timer->update();
		_renderTimer.update();


		handleUserInput();
		handleEvents();

		tryUpdate(); // updating is bound by MS_PER_UPDATE

		tryRender(); // rendering is bound by MS_PER_RENDER
	}
}

// handle game events
void Game::handleEvents() {
	auto ev = EventQueue::getInstance().poll(EventType::GAME_NEW);
	if (ev.type == EventType::GAME_NEW) {
		// reinitialize world
		_world->free();
		init();
	}

	ev = EventQueue::getInstance().poll(EventType::GAME_LOGIC_PAUSE);
	if (ev.type == EventType::GAME_LOGIC_PAUSE) {
		_timer->pause();
	}

	ev = EventQueue::getInstance().poll(EventType::GAME_LOGIC_UNPAUSE);
	if (ev.type == EventType::GAME_LOGIC_UNPAUSE) {
		_timer->resume();
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

