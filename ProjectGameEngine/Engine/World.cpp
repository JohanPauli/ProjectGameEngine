#include "World.h"

#include "Entity.h"
#include "InputMapping.h"
#include "InputComponent.h"
#include "Physics.h"


// deallocate all entities
inline void deleteCont(EntityCont& container) {
	for (auto entity : container) {
		delete entity;
	}
}
inline void deleteEntityList(EntityList& el) {
	deleteCont(el.backgroundEntities);
	deleteCont(el.dynamicEntities);
	deleteCont(el.foregroundEntities);
	deleteCont(el.staticEntities);
}


World::~World() {
	// delete active entities
	deleteEntityList(_activeEntities);

	// delete inactive entities	
	deleteEntityList(_inactiveEntitiesLeft);
	deleteEntityList(_inactiveEntitiesRight);


	// borders
	delete _botBorder;
	delete _topBorder;

	// player
	delete _player;
}


// update
inline void updateContainer(EntityCont& container, int x, int y) {
	for (auto entity : container) {
		entity->update(x, y);
	}
}
inline void updateEntityList(EntityList& el, int xOffset, int yOffset) {
	updateContainer(el.backgroundEntities, xOffset, yOffset);
	updateContainer(el.staticEntities, xOffset, yOffset);
	updateContainer(el.dynamicEntities, xOffset, yOffset);
	updateContainer(el.foregroundEntities, xOffset, yOffset);
}


void World::update() {
	// active
	updateEntityList(_activeEntities, 0,0);
	_player->update(0,0);

	// inactive
	updateEntityList(_inactiveEntitiesLeft, 0,0);
	updateEntityList(_inactiveEntitiesRight, 0,0);

	// check collision
}


// render an EntityCont
inline void renderContainer(EntityCont& container, Renderer* renderer) {
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


// check if entity is left of the offSet
inline bool toTheLeft(Entity* entity, int xOffset) {
	return (entity->_physics->getXPosition() < (float)xOffset);
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
		InputMapper::getInstance().deactivateContext(_player->_input->getInputContextId());
		delete _player;
	}
	_player = entity;
	// activate new player's input
	// assume the entity is already registered
	InputMapper::getInstance().activateContext(_player->_input->getInputContextId());
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