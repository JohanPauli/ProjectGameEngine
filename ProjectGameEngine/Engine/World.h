#pragma once

#include <deque>
#include "Level.h"
#include "Physics.h"
#include "Counter.h"
#include "InputContext.h"
#include "ScoreScreen.h"

class Entity;
class Renderer;

typedef std::deque<Entity*> EntityCont;


// the purpose of the entity
enum class EntityType {
	DYNAMIC,
	STATIC,
	BACKGROUND,
	FOREGROUND,
	PLAYER,
	BORDER_BOTTOM,
	BORDER_TOP
};


// containers containers for all entity types
struct EntityList {
	EntityCont dynamicEntities;
	EntityCont staticEntities;
	EntityCont backgroundEntities;
	EntityCont foregroundEntities;
};



class World : InputContext {
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
	bool		_gameOver = false;

	Counter*	_counter;
	ScoreScreen* _scoreScreen;

private:
	// move the display area to where the player is
	void setPlayer(Entity* player);
	void followPlayer();
	void endGame();
	void calcScore();

public:
	World(int width, int height);
	~World();

	// get input when world is paused
	virtual bool onNotify(const KeyboardInput&);

	// deallocate all objects and resets the world state
	void free();

	// freeze the game world
	void pause();
	
	// unfreeze the game world
	void unpause();

	// end game
	void gameOver();

	//Load world
	void init(Level level);

	// update logic
	void update();

	// render the entities
	void render(Renderer*);

	// add non-player non-border entities
	void addEntity(Entity* entity, EntityType type);

	// activate an entity
	bool activateLeftEntity(EntityType type);

	// activate an entity
	bool activateRightEntity(EntityType type); 

	// deactivate an entity
	bool deactivateLeftEntity(EntityType type);

	// deactivate an entity
	bool deactivateRightEntity(EntityType type);
};