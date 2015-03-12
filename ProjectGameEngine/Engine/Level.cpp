#include "Level.h"
#include "Game.h"
#include "RessourceManager.h"
#include "Physics.h"
#include "GraphicsComponent.h"
#include "EntityGenerators.h"

Level::Level(int width, int height) : width(width), height(height)
{
	//if(!init())
		//Chrash!!!
	init();
}



/*
Level components are hardcoded here
Hopefully functionality can be implemented to 
read from file instead
*/
bool Level::init()
{
	RessourceManager &rManager = RessourceManager::getInstance();

	//second draft
		std::random_device rd;
		std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis((height*0.1), (height*0.50));
	for (int xPos = 500; xPos < 10000; xPos += 500)
		{
		int yPos = dis(gen);
		auto pipePair = EntityGenerator::getInstance().generatePipes(xPos, yPos, 230);
		pipes.push_back(pipePair.first);
		pipes.push_back(pipePair.second);
		}
	auto bg = EntityGenerator::getInstance().generateBackground();
	auto fg = EntityGenerator::getInstance().generateForeground();

	// order matters. first added is last rendererd
	background.push_back(fg);
	background.push_back(bg);
	player = EntityGenerator::getInstance().generatePlayerBird();

	return true;
		
}
//Game world will call this to set game player
Entity* Level::getPlayer() const
{
	return player;
}

std::vector<Entity*>& Level::getPipeEntities()
{
	return pipes;
}

std::vector<Entity*>& Level::getBackground()
{
	return background;
}

std::vector<Entity*>& Level::getForeground()
{
	return foreground;
}