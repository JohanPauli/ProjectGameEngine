#include "World.h"

#include "Debug.h"
#include "Entity.h"
#include "InputMapping.h"
#include "InputComponent.h"
#include "Rendering.h"



World::World(int width, int height) 
: _displayWidth(width), _displayHeight(height) {}


World::~World() {
	free();
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
	// active
	updateEntityList(_activeEntities);
	if (_player != nullptr)
		_player->update();
	if (_background != nullptr)
		_background->update();

	// inactive
	updateEntityList(_inactiveEntitiesLeft);
	updateEntityList(_inactiveEntitiesRight);

	// check collision
	_physEng.detectCollisions(_player, _activeEntities.staticEntities);
	_physEng.detectCollisions(_player, _activeEntities.dynamicEntities);
	_physEng.detectCollisions(_player, _botBorder);
	_physEng.detectCollisions(_player, _topBorder);
	_physEng.detectCollisions(_activeEntities.dynamicEntities, _activeEntities.staticEntities);
	_physEng.detectCollisions(_activeEntities.dynamicEntities, _activeEntities.dynamicEntities);

	// TODO: dynamicentities can also collide with borders, implement it!
}


// render an EntityCont
inline void renderContainer(EntityCont& container, Renderer* renderer) {
	for (auto entity : container) {
		entity->render(renderer);
	}
}


void World::init(Level level)
{
	std::vector<Entity*> pipes = level.getPipeEntities();
	for (auto it = pipes.begin(); it != pipes.end(); it++)
	{
		addEntity( *it, EntityType::STATIC);
	}

	std::vector<Entity*> back = level.getBackground();
	for (auto it = back.begin(); it != back.end(); it++)
	{
		addEntity(*it, EntityType::BACKGROUND);
	}

	std::vector<Entity*> foreground = level.getForeground();
	for (auto it = foreground.begin(); it != foreground.end(); it++)
	{
		addEntity(*it, EntityType::FOREGROUND);
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
	manageScene();
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

void World::manageScene() {
	// TODO: add code
}