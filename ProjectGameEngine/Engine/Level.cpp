#include "Level.h"

Level::Level(Renderer *renderer)
{
	//if(!init())
		//Chrash!!!
	init(renderer);
}

/*
Level components are hardcoded here
Hopefully functionality can be implemented to 
read from file instead
*/
bool Level::init(Renderer *renderer)
{

	int windowHeight = Game::getWindowHeight();
	int windowWidth = Game::getWindowWidth();
	RessourceManager &rManager = RessourceManager::getInstance(renderer);
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
		auto pipeGraphics = new PipeGraphics(pipeBot, pipeMid, false);
		auto pipeGraphics2 = new PipeGraphics(pipeBot, pipeMid, true);

		//First draft
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(100, Game::getWindowHeight() / 2);
		for (float xPos = 200; xPos < 1000; xPos += 100)
		{
			float yPos = (float)dis(gen);
			auto pipePhyTop = new StaticPhysics(0.f, 0.f, -1.f, 0.f, xPos, yPos, 240.f, 130.f);
			auto pipePhyBot = new StaticPhysics(0, 0, -1, 0, xPos, yPos + 50, 478, 130);
			pipes.push_back(new Entity(pipePhyTop, pipeGraphics));
			pipes.push_back(new Entity(pipePhyBot, pipeGraphics2));

		}

	}
	else
		return false;
	if (backgroundSky && backgroundLand){
		float skyScale = (windowHeight / backgroundSky->getHeight());
		auto bgPhy = new StaticPhysics(0.f, 0.f, -0.1, 0.f, 0.f, 0.f, skyScale * backgroundSky->getHeight(), skyScale * backgroundSky->getWidth());
		BackgroundGraphics *bgGraphics = new BackgroundGraphics(backgroundSky, backgroundLand);
		background.push_back(new Entity(bgPhy, bgGraphics));
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

std::vector<Entity*> Level::getPipeEntities() const
{
	return pipes;
}