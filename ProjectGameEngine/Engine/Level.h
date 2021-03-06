#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.h"

#include "Rendering.h"

#include <vector>
#include <queue>
#include <random>

/*
This class will be used to initialize all game objects
*/
class Level
{
private:
	std::vector < Entity*> pipes;
	std::vector < Entity* > background;
	std::vector < Entity* > foreground;
	Entity*	player;
	Entity*	topBorder;
	Entity*	botBorder;
	
	// window size
	int width;
	int height;

	bool init();
	
public:
	Level(int width, int height);

	std::vector<Entity*>& getPipeEntities();
	std::vector<Entity*>& getBackground();
	std::vector<Entity*>& getForeground();
	Entity* getPlayer() const;
	Entity* getBorderTop() const;
	Entity* getBorderBottom() const;
};

#endif