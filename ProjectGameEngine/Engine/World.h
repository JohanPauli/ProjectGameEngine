#pragma once

#include <deque>

class Entity;
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


// containers containers for all entity types
struct EntityList {
	EntityCont dynamicEntities;
	EntityCont staticEntities;
	EntityCont backgroundEntities;
	EntityCont foregroundEntities;
};



class World {
private:
	EntityList	_activeEntities;		// entities to get rendered
	EntityList	_inactiveEntitiesLeft;	// entities to the left of the screen
	EntityList	_inactiveEntitiesRight;	// entities to the right of the screen
	Entity*		_player;
	Entity*		_botBorder;
	Entity*		_topBorder;
	int		_xOffset = 0;
	int		_yOffset = 0;

private:


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
	bool activateLeftEntity(EntityType type);

	// activate an entity
	bool activateRightEntity(EntityType type); 

	// deactivate an entity
	bool deactivateLeftEntity(EntityType type);

	// deactivate an entity
	bool deactivateRightEntity(EntityType type);
};