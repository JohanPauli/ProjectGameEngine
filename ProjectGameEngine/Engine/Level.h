#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.h"
#include "Physics.h"
#include "GraphicsComponent.h"
#include "Rendering.h"
#include "Game.h"
#include "RessourceManager.h"
#include <vector>
#include <queue>
#include <random>

/*
This class will be used to initialize all game objects
*/
class Level
{
private:
	Entity *player;
	std::vector < Entity* > pipes;
	std::vector < Entity* > background;

	bool init(Renderer*);
	
public:
	Level(Renderer *renderer);

	std::vector<Entity*> getPipeEntities() const;
	std::vector<Entity*> getBackground() const;
	//Entity* getPlayer() const;

};

#endif