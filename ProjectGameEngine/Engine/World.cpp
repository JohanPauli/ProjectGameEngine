#include "World.h"

#include "InputMapping.h"
#include "InputComponent.h"


inline void deleteCont(EntityCont container) {
	for (auto entity : container) {
		delete entity;
	}
}


World::~World() {
	// delete active entities
	deleteCont(_activeEntities.backgroundEntities);
	deleteCont(_activeEntities.dynamicEntities);
	deleteCont(_activeEntities.foregroundEntities);
	deleteCont(_activeEntities.staticEntities);

	// delete inactive entities
	deleteCont(_inactiveEntities.backgroundEntities);
	deleteCont(_inactiveEntities.dynamicEntities);
	deleteCont(_inactiveEntities.foregroundEntities);
	deleteCont(_inactiveEntities.staticEntities);

	// borders
	delete _botBorder;
	delete _topBorder;
	delete _player;
}


inline void updateContainer(EntityCont container, int x, int y) {
	for (auto entity : container) {
		entity->update(x, y);
	}
}


void World::update() {
	updateContainer(_activeEntities.backgroundEntities, _xOffset, _yOffset);
	updateContainer(_activeEntities.staticEntities, _xOffset, _yOffset);
	updateContainer(_activeEntities.dynamicEntities, _xOffset, _yOffset);
	updateContainer(_activeEntities.foregroundEntities, _xOffset, _yOffset);
	_player->update(_xOffset, _yOffset);

	// check collision

	updateContainer(_inactiveEntities.backgroundEntities, _xOffset, _yOffset);
	updateContainer(_inactiveEntities.staticEntities, _xOffset, _yOffset);
	updateContainer(_inactiveEntities.dynamicEntities, _xOffset, _yOffset);
	updateContainer(_inactiveEntities.foregroundEntities, _xOffset, _yOffset);
}


inline void renderContainer(EntityCont container, Renderer* renderer) {
	for (auto entity : container) {
		entity->render(renderer);
	}
}


void World::render(Renderer* renderer) {
	renderContainer(_activeEntities.backgroundEntities, renderer);
	renderContainer(_activeEntities.dynamicEntities, renderer);
	_player->render(renderer);
	renderContainer(_activeEntities.staticEntities, renderer);
	renderContainer(_activeEntities.foregroundEntities, renderer);
}


void World::addEntity(Entity* entity, EntityType eType) {
	switch (eType) {
	case EntityType::DYNAMIC:
		_inactiveEntities.dynamicEntities.emplace_front(entity);
		break;
	case EntityType::STATIC:
		_inactiveEntities.staticEntities.emplace_front(entity);
		break;
	case EntityType::BACKGROUND:
		_inactiveEntities.backgroundEntities.emplace_front(entity);
		break;
	case EntityType::FOREGROUND:
		_inactiveEntities.foregroundEntities.emplace_front(entity);
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
	if (_player != nullptr)
		InputMapper::getInstance().deactivateContext(_player->_input->getInputContextId());
	_player = entity;
	// activate new player's input
	// assume the entity is already registered
	InputMapper::getInstance().activateContext(_player->_input->getInputContextId());
}


inline bool moveBackEntity(EntityCont* from, EntityCont* to) {
	if (from->empty())
		return false;
	auto entity = from->back();
	from->pop_back();
	to->emplace_front(entity);
	return true;
}

inline bool moveFrontEntity(EntityCont* from, EntityCont* to) {
	if (from->empty())
		return false;
	auto entity = from->front();
	from->pop_front();
	to->emplace_back(entity);
	return true;
}

bool World::activateFrontEntity(EntityType type) {
	switch (type) {
	case EntityType::DYNAMIC: 
		return moveBackEntity(&_inactiveEntities.dynamicEntities, &_activeEntities.dynamicEntities);
	case EntityType::STATIC:
		return moveBackEntity(&_inactiveEntities.staticEntities, &_activeEntities.staticEntities);
	case EntityType::BACKGROUND:
		return moveBackEntity(&_inactiveEntities.backgroundEntities, &_activeEntities.backgroundEntities);
	case EntityType::FOREGROUND:
		return moveBackEntity(&_inactiveEntities.foregroundEntities, &_activeEntities.foregroundEntities);
	default: return false;
	}
}

bool World::activateBackEntity(EntityType type) {
	return false;
}

void World::deactivateFrontEntity(EntityType type) {

}
void World::deactivateBackEntity(EntityType type) {

}