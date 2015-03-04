#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "Entity.h"

// forward declarations
class Rect;
class Sprite;
class SpriteSheet;
class Physics;
class DynamicPhysics;
class StaticPhysics;


/*
	class representing the player
*/
class PlayerEntity : public Entity {
private:
	DynamicPhysics* _physics = nullptr;
	SpriteSheet*	_spriteSheet = nullptr;
	int				_updates = 0;


public:
	PlayerEntity(DynamicPhysics* physics, Sprite* sprite);
	virtual ~PlayerEntity();
	
	virtual void update() override;

	virtual void render(Renderer* renderer) override;

	virtual void jump();
};


/*
	PipeEntity 
*/
class PipeEntity : public Entity {
private:
	StaticPhysics*	_physics = nullptr;
	Sprite*			_top = nullptr;	// 26x13
	Sprite*			_mid = nullptr;	// 26x1
	Sprite*			_bot = nullptr;	// 26x13

public:
	const static int HEIGHT_MIN = 26;

public:
	PipeEntity(StaticPhysics* physics, Sprite* bot, Sprite* mid, Sprite* top);
	virtual ~PipeEntity();

	virtual void update() override;

	virtual void render(Renderer* renderer) override;
};


#endif