#pragma once


#include "Entity.h"
#include "Physics.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "InputMapping.h"
#include "RessourceManager.h"
#include "Timer.h"

class EntityGenerator {
private:
	Timer*	_timer = nullptr;
	int		_dispWidth = 0;
	int		_dispHeight = 0;

	// singleton stuff
private:
	EntityGenerator() {}
	EntityGenerator(const EntityGenerator&) = delete;
	EntityGenerator& operator= (const EntityGenerator&) = delete;

public:
	static EntityGenerator& getInstance() {
		static EntityGenerator instance;
		return instance;
	}

	void init(int width, int height, Timer* timer) {
		_dispWidth = width; 
		_dispHeight = height; 
		_timer = timer;
	}

	// generates the bird player
	Entity* generatePlayerBird() {
		auto physics = new DynamicPhysics(0.f, 0.1f, 2.5f, 0.f, 
			0.f, 200.f,  // x, y
			50.f, 70.f); // h, w
		
		// graphics component
		// create bird spritesheet
		std::vector<Rect> rector;
		rector.emplace_back(Rect(0, 0, 18, 12));
		rector.emplace_back(Rect(0, 0, 18, 12));
		rector.emplace_back(Rect(18, 0, 18, 12));
		rector.emplace_back(Rect(36, 0, 18, 12));
		rector.emplace_back(Rect(36, 0, 18, 12));
		rector.emplace_back(Rect(18, 0, 18, 12));
		auto spriteSheet = new SpriteSheet(rector, RessourceManager::getInstance().getByTag<Sprite*>("bird"));
		auto uTimer = UpdateTimer(_timer, 250);
		auto graphics = new BirdGraphics(spriteSheet, uTimer);

		// input component
		auto input = new PlayerInput();
		InputMapper::getInstance().registerContext(input);

		return new Entity(physics, graphics, input);
	}

	// generates a top and bottom PipeEntity
	std::pair<Entity*, Entity*> generatePipes(int xPos, int gapPos, int gapHeight) {
		auto spriteBot = RessourceManager::getInstance().getByTag<Sprite*>("pipebot");
		auto spriteMid= RessourceManager::getInstance().getByTag<Sprite*>("pipemid");
		auto botPipeGraphics = new PipeGraphics(spriteBot, spriteMid, true);
		auto topPipeGraphics = new PipeGraphics(spriteBot, spriteMid, false);
		int gapEndY = gapPos + gapHeight;

		auto topPhysics = new StaticPhysics(0.f, 0.f, 0.f, 0.f,
			(float)xPos, 0.f,
			(float)gapPos, 130.f);
		auto botPhysics = new StaticPhysics(0.f, 0.f, 0.f, 0.f,
			(float)xPos, (float)gapEndY,
			(float)(_dispHeight - gapEndY), 130.f);
		
		auto botEntity = new Entity(botPhysics, botPipeGraphics);
		auto topEntity = new Entity(topPhysics, topPipeGraphics);

		return std::make_pair(botEntity, topEntity);
	}

	Entity* generateBackground() {
		auto bgSprite = RessourceManager::getInstance().getByTag<Sprite*>("sky");
		auto bgGraphics = new BackgroundGraphics2(bgSprite, 4, 8);
		auto bgPhy = new StaticPhysics(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, _dispHeight, _dispWidth);
		return new Entity(bgPhy, bgGraphics);
	}

	Entity* generateForeground() {
		auto fgSprite = RessourceManager::getInstance().getByTag<Sprite*>("land");
		auto bgGraphics = new ForegroundGraphics(fgSprite, 1.5, 2);
		auto bgPhy = new StaticPhysics(0.f, 0.f, 0.f, 0.f, 
			0.f, _dispHeight, _dispHeight, _dispWidth);

		return new Entity(bgPhy, bgGraphics);
	}

	Entity* generateBorderBottom() {
		auto physics = new StaticPhysics(0.f, 0.f, 0.f, 0.f,
			FLT_MIN, _dispHeight, FLT_MAX, FLT_MAX);
		return new Entity(physics);
	}


	Entity* generateBorderTop() {
		auto physics = new StaticPhysics(0.f, 0.f, 0.f, 0.f,
			FLT_MIN, FLT_MIN, -FLT_MIN, FLT_MAX);
		return new Entity(physics);
	}
};