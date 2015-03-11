#pragma once

#include <deque>
#include "Level.h"
#include "Physics.h"

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
	PhysicsEngine _physEng;
	EntityList	_activeEntities;		// entities to get rendered
	EntityList	_inactiveEntitiesLeft;	// entities to the left of the screen
	EntityList	_inactiveEntitiesRight;	// entities to the right of the screen
	Entity*		_player = nullptr;
	Entity*		_botBorder = nullptr;
	Entity*		_topBorder = nullptr;
	int			_displayWidth;
	int			_displayHeight;
	int			_xOffset = 0;
	int			_yOffset = 0;

private:
	// move the display area to where the player is
	void followPlayer();

public:
	World(int width, int height);
	~World();

	// deallocate all objects and resets the world state
	void free();


	//Load world
	void init(Level level);

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

	void manageScene();
};