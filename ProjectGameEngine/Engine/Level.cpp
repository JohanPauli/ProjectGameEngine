#include "Level.h"
#include "Game.h"
#include "RessourceManager.h"
#include "Physics.h"
#include "GraphicsComponent.h"

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
	Sprite*	bird = nullptr;
	Sprite*	pipeTop = nullptr;
	Sprite*	pipeMid = nullptr;
	Sprite*	pipeBot = nullptr;
	Sprite* backgroundSky = nullptr;
	Sprite* backgroundLand = nullptr;

	/*Sprites*/
	bird = rManager.getByTag<Sprite*>("bird");
	pipeMid = rManager.getByTag<Sprite*>("pipemid");
	pipeBot = rManager.getByTag<Sprite*>("pipebot");
	backgroundSky = rManager.getByTag<Sprite*>("sky");
	backgroundLand = rManager.getByTag<Sprite*>("land");

	if (pipeMid && pipeBot){
		PipeGraphics* pipeGraphics;
		PipeGraphics* pipeGraphics2;
		StaticPhysics *top;
		StaticPhysics *bot;

		//First draft
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(100, (height*0.75-350));
		for (float xPos = 200; xPos < 10000; xPos += 500)
		{
			float yPos = (float)dis(gen);
			pipeGraphics = new PipeGraphics(pipeBot, pipeMid, false);
			pipeGraphics2 = new PipeGraphics(pipeBot, pipeMid, true);
			top = new StaticPhysics(0.f, 0.f, -1.f, 0.f, xPos, 0, yPos, 130.f);
			bot = new StaticPhysics(0, 0, -1, 0, xPos, yPos + 350, height/2, 130);
			pipes.push_back(new Entity(top, pipeGraphics));
			pipes.push_back(new Entity(bot, pipeGraphics2));

		}

	}
	else
		return false;
	if (backgroundSky && backgroundLand){
		//skalera bakgrund so hon gongur frá ovast til har landbakgrund byrjar
		float skyScale = (((float)height - backgroundLand->getHeight()) / backgroundSky->getHeight());

		float xPos = 0;
		BackgroundGraphics *bgGraphics;

		//Make as many background entities as it takes to cover the window
		while (xPos < width)
		{
			bgGraphics = new BackgroundGraphics(backgroundSky);
			auto bgPhy = new StaticPhysics(xPos, 0.f, -0.1, 0.f, 0.f, 0.f, skyScale * backgroundSky->getHeight(), skyScale * backgroundSky->getWidth());
			background.push_back(new Entity(bgPhy, bgGraphics));

			xPos += skyScale * backgroundSky->getWidth();

		}

		xPos = 0;
		int yPos = skyScale * backgroundSky->getHeight();

		//same as with backgroundLand
		while (xPos < width)
		{
			bgGraphics = new BackgroundGraphics(backgroundLand);
			auto bgPhy = new StaticPhysics(0.f, 0.f, -1.f, 0.f, xPos, yPos, backgroundLand->getHeight(), backgroundLand->getWidth());
			foreground.push_back(new Entity(bgPhy, bgGraphics));

			xPos += backgroundLand->getWidth();

		}
		


	}
	else
		return false;

	return true;
		
}
//Game world will call this to set game player
/*PlayerEntity* Level::getPlayer() const
{
	return player;
}*/

/*PhysicsEngine* Level::getPhysicsEngine() const
{
	return pEngine;
}*/

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