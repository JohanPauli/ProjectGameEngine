#pragma once

#include <deque>

#include "Entity.h"

class Renderer;

typedef std::deque<Entity*> EntityCont;


// the purpose of the entity
enum class EntityType {
	DYNAMIC,
	STATIC,
	BACKGROUND,
	FOREGROUND,
	PLAYER
};

//
struct EntityList {
	EntityCont dynamicEntities;
	EntityCont staticEntities;
	EntityCont backgroundEntities;
	EntityCont foregroundEntities;
};



class World {
private:
	EntityList	_activeEntities;	// entities off screen
	EntityList	_inactiveEntities;	// entities shown on screen
	Entity*		_player;
	Entity*		_botBorder;
	Entity*		_topBorder;
	int		_xOffset = 0;
	int		_yOffset = 0;

public:
	~World();
	// update logic
	void update();

	// render the entities
	void render(Renderer*);

	// add non-player non-border entities
	void addEntity(Entity* entity, EntityType type);

	// set top and bottom borders
	void setBorders(Entity* top, Entity* bottom);

	// set player
	void setPlayer(Entity* player);

	// activate an entity
	bool activateFrontEntity(EntityType type);

	// activate an entity
	bool activateBackEntity(EntityType type);

	void deactivateFrontEntity(EntityType type);
	void deactivateBackEntity(EntityType type);
};