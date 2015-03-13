#include "World.h"

#include "Debug.h"
#include "Entity.h"
#include "InputMapping.h"
#include "InputComponent.h"
#include "Rendering.h"
#include "Events.h"



World::World(int width, int height) 
: _displayWidth(width), _displayHeight(height) {
	InputMapper::getInstance().registerContext(this);
}


World::~World() {
	free();
	InputMapper::getInstance().unregisterContext(this);
}


// deallocate all entities
inline void deleteCont(EntityCont& container) {
	for (auto entity : container) {
		delete entity;
		entity = nullptr;
	}
}
inline void deleteEntityList(EntityList& el) {
	deleteCont(el.backgroundEntities);
	el.backgroundEntities.clear();
	deleteCont(el.dynamicEntities);
	el.dynamicEntities.clear();
	deleteCont(el.foregroundEntities);
	el.foregroundEntities.clear();
	deleteCont(el.staticEntities);
	el.staticEntities.clear();
}

bool World::onNotify(const KeyboardInput& input) {
	// this context should only be active when the game is paused
	switch (input.key) {
	case KeyboardKey::SPACE:
		unpause();
		return true;
	}
	return false;
}


void World::free() {
	// delete active entities
	deleteEntityList(_activeEntities);

	// delete inactive entities	
	deleteEntityList(_inactiveEntitiesLeft);
	deleteEntityList(_inactiveEntitiesRight);

	delete _background; _background = nullptr;

	// borders
	delete _botBorder; _botBorder = nullptr;
	delete _topBorder; _topBorder = nullptr;

	// player
	delete _player; _player = nullptr;
	_xOffset = 0; 
	_yOffset = 0;

	delete _counter; _counter = nullptr;
	delete _scoreScreen; _scoreScreen = nullptr;
}



void World::init(Level level) {
	// deallocate previous run if any
	free();

	// load pipes
	std::vector<Entity*> pipes = level.getPipeEntities();
	for (auto pipe : pipes)
	{
		addEntity(pipe, EntityType::STATIC);
	}

	// load backgrounds
	std::vector<Entity*> backgrounds = level.getBackground();
	for (auto background : backgrounds)
	{
		addEntity(background, EntityType::BACKGROUND);
}

	// load foregrounds
	std::vector<Entity*> foregrounds = level.getForeground();
	for (auto foreground : foregrounds)
	{
		addEntity(foreground, EntityType::FOREGROUND);
	}
	setPlayer(level.getPlayer());
	setBorders(level.getBorderTop(), level.getBorderBottom());

	_counter = new Counter(10, 10, 4);
	_scoreScreen = new ScoreScreen();

	// activate entities
	while (activateLeftEntity(EntityType::STATIC));
	while (activateRightEntity(EntityType::STATIC));
	while (activateLeftEntity(EntityType::BACKGROUND));
	while (activateRightEntity(EntityType::BACKGROUND));
	while (activateLeftEntity(EntityType::FOREGROUND));
	while (activateRightEntity(EntityType::FOREGROUND));

}

// freeze the game world
void World::pause() {
	EventQueue::getInstance().add(GameEvent(EventType::GAME_LOGIC_PAUSE));
	if (_player != nullptr)
		InputMapper::getInstance().deactivateContext(_player->getInputContext());
	InputMapper::getInstance().activateContext(this);
}

// unfreeze the game world
void World::unpause() {
	EventQueue::getInstance().add(GameEvent(EventType::GAME_LOGIC_UNPAUSE));
	InputMapper::getInstance().deactivateContext(this);
	if (_player != nullptr)
		InputMapper::getInstance().activateContext(_player->getInputContext());
	if (_gameOver)
		EventQueue::getInstance().add(GameEvent(EventType::GAME_NEW));
}


// end game
void World::gameOver() {
	pause();
	_scoreScreen->setScore(_counter->getNumber());
}



// update
inline void updateContainer(EntityCont& container) {
	for (auto entity : container) {
		entity->update();
	}
}
inline void updateEntityList(EntityList& el) {
	updateContainer(el.backgroundEntities);
	updateContainer(el.staticEntities);
	updateContainer(el.dynamicEntities);
	updateContainer(el.foregroundEntities);
}


void World::update() {
	// get offsets
	followPlayer();

	// update active
	updateEntityList(_activeEntities);
	if (_player != nullptr)
		_player->update();
	if (_background != nullptr)
		_background->update();

	// update inactive
	updateEntityList(_inactiveEntitiesLeft);
	updateEntityList(_inactiveEntitiesRight);


	if (_physEng.detectCollisions(_player, _activeEntities.staticEntities))
		_gameOver = true;
	if (_physEng.detectCollisions(_player, _activeEntities.dynamicEntities))
		_gameOver = true;
	if (_botBorder != nullptr)
		if (_physEng.detectCollisions(_player, _botBorder))
			_gameOver = true;
	if (_topBorder != nullptr)
		if (_physEng.detectCollisions(_player, _topBorder))
			_gameOver = true;
	
	// check other collisions
	_physEng.detectCollisions(_activeEntities.dynamicEntities, _activeEntities.staticEntities);
	_physEng.detectCollisions(_activeEntities.dynamicEntities, _activeEntities.dynamicEntities);

	// end game if player collided
	if (_gameOver)
		gameOver();
	// TODO: make dynamicentities able to collide with borders, 
	// need to overload detectcollision with dequeue as first param and entity as second
}


// render an EntityCont
inline void renderContainer(EntityCont& container, Renderer* renderer) {
	for (auto entity : container) {
		entity->render(renderer);
		
	}
}



void World::render(Renderer* renderer) {
	// rendering offsets

	renderer->setOffsets(_xOffset, _yOffset);
	if (_background != nullptr)
		_background->render(renderer);
	renderContainer(_activeEntities.backgroundEntities, renderer);
	renderContainer(_activeEntities.dynamicEntities, renderer);
	if (_player != nullptr)
		_player->render(renderer);
	renderContainer(_activeEntities.staticEntities, renderer);
	renderContainer(_activeEntities.foregroundEntities, renderer);

	//counter position is fixed
	renderer->setOffsets(0, 0);
	if (_counter != nullptr)
		_counter->render(renderer);
	if (_scoreScreen != nullptr)
		_scoreScreen->render(renderer);
	calcScore();
}


// check if entity is left of the offSet
inline bool toTheLeft(Entity* entity, int xOffset) {
	return (entity->getX() < (float)xOffset);
}
inline void emplaceEntity(Entity* entity, EntityCont& left, EntityCont& right, int xOffset) {
	if (toTheLeft(entity, xOffset))
		left.emplace_back(entity);
	else
		right.emplace_back(entity);
}


void World::addEntity(Entity* entity, EntityType eType) {
	switch (eType) {
	case EntityType::DYNAMIC:
		emplaceEntity(entity, _inactiveEntitiesLeft.dynamicEntities, _inactiveEntitiesRight.dynamicEntities, _xOffset);
		break;
	case EntityType::STATIC:
		emplaceEntity(entity, _inactiveEntitiesLeft.staticEntities, _inactiveEntitiesRight.staticEntities, _xOffset);
		break;
	case EntityType::BACKGROUND:
		emplaceEntity(entity, _inactiveEntitiesLeft.backgroundEntities, _inactiveEntitiesRight.backgroundEntities, _xOffset);
		break;
	case EntityType::FOREGROUND:
		emplaceEntity(entity, _inactiveEntitiesLeft.foregroundEntities, _inactiveEntitiesRight.foregroundEntities, _xOffset);
		break;
	case EntityType::PLAYER:
		setPlayer(entity);
		break;
	default:break;
	}
}


void World::setBorders(Entity* top, Entity* bot) {
	_topBorder = top;
	_botBorder = bot;
}


void World::setPlayer(Entity* entity) {
	// deactivate old player's input
	if (_player != nullptr) {
		InputMapper::getInstance().deactivateContext(_player->getInputContext());
		delete _player;
	}
	_player = entity;
	// activate new player's input
	// assume the entity is already registered
	if (entity != nullptr)
	InputMapper::getInstance().activateContext(_player->getInputContext());
}


inline bool activateEntity(EntityCont& inactive, EntityCont& active, bool left) {
	if (inactive.empty())
		return false;
	auto entity = inactive.back();
	inactive.pop_back();
	if (left)
		active.emplace_front(entity);
	else
		active.emplace_back(entity);
	return true;
}


inline bool deactivateEntity(EntityCont& active, EntityCont& inactive, bool left) {
	if (active.empty())
		return false;
	Entity* entity;
	if (left) {
		entity = active.front();
		active.pop_front();
	}
	else {
		entity = active.back();
		active.pop_back();
	}
	inactive.emplace_front(entity);
	return true;
}



bool World::activateLeftEntity(EntityType type) {
	switch (type) {
	case EntityType::DYNAMIC:
		return activateEntity(_inactiveEntitiesLeft.dynamicEntities, _activeEntities.dynamicEntities, true);
	case EntityType::STATIC:
		return activateEntity(_inactiveEntitiesLeft.staticEntities, _activeEntities.staticEntities, true);
	case EntityType::BACKGROUND:
		return activateEntity(_inactiveEntitiesLeft.backgroundEntities, _activeEntities.backgroundEntities, true);
	case EntityType::FOREGROUND:
		return activateEntity(_inactiveEntitiesLeft.foregroundEntities, _activeEntities.foregroundEntities, true);
	default: return false;
	}
}



bool World::activateRightEntity(EntityType type) {
	switch (type) {
	case EntityType::DYNAMIC: 
		return activateEntity(_inactiveEntitiesRight.dynamicEntities, _activeEntities.dynamicEntities, false);
	case EntityType::STATIC:
		return activateEntity(_inactiveEntitiesRight.staticEntities, _activeEntities.staticEntities, false);
	case EntityType::BACKGROUND:
		return activateEntity(_inactiveEntitiesRight.backgroundEntities, _activeEntities.backgroundEntities, false);
	case EntityType::FOREGROUND:
		return activateEntity(_inactiveEntitiesRight.foregroundEntities, _activeEntities.foregroundEntities, false);
	default: return false;
	}
}





bool World::deactivateLeftEntity(EntityType type) {
	switch (type) {
	case EntityType::DYNAMIC:
		return deactivateEntity(_activeEntities.dynamicEntities, _inactiveEntitiesLeft.dynamicEntities, true);
	case EntityType::STATIC:
		return deactivateEntity(_activeEntities.staticEntities, _inactiveEntitiesLeft.staticEntities, true);
	case EntityType::BACKGROUND:
		return deactivateEntity(_activeEntities.backgroundEntities, _inactiveEntitiesLeft.backgroundEntities, true);
	case EntityType::FOREGROUND:
		return deactivateEntity(_activeEntities.foregroundEntities, _inactiveEntitiesLeft.foregroundEntities, true);
	default: return false;
	}
}


bool World::deactivateRightEntity(EntityType type) {
	switch (type) {
	case EntityType::DYNAMIC:
		return deactivateEntity(_activeEntities.dynamicEntities, _inactiveEntitiesLeft.dynamicEntities, false);
	case EntityType::STATIC:
		return deactivateEntity(_activeEntities.staticEntities, _inactiveEntitiesLeft.staticEntities, false);
	case EntityType::BACKGROUND:
		return deactivateEntity(_activeEntities.backgroundEntities, _inactiveEntitiesLeft.backgroundEntities, false);
	case EntityType::FOREGROUND:
		return deactivateEntity(_activeEntities.foregroundEntities, _inactiveEntitiesLeft.foregroundEntities, false);
	default: return false;
	}
}


void World::followPlayer() {
	if (_player == nullptr)
		return;
	_xOffset = 
		(int)_player->getX() 
		+ (int)(_player->getWidth() / 2) 
		- (int)(_displayWidth / 2);
	_yOffset = 0;
}


void World::calcScore()
{
	if (_activeEntities.staticEntities.empty())
		return;
	int score = (int)_inactiveEntitiesLeft.staticEntities.size();
	int idx = (int)_activeEntities.staticEntities.size();
	Entity* pipe;

	do 
	{
		idx--;
		pipe = _activeEntities.staticEntities.at(idx);
		if (_player->getX() > pipe->getX())
			score++;


	} while (_player->getX() > pipe->getX() && idx > 0);
	_counter->setNumber(score/2);
	
	
}