#pragma once

#include "Entity.h"
#include "Rect.h"
#include "InputContext.h"	// input handling


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
class PlayerEntity : public Entity, public InputContext {
private:
	DynamicPhysics* _physics = nullptr;
	SpriteSheet*	_spriteSheet = nullptr;
	int				_updates = 0;

private:
	void flap(bool repeat);

public:
	PlayerEntity(DynamicPhysics* physics, SpriteSheet* sprite);
	virtual ~PlayerEntity();

	// Entity: updates logic
	virtual void update() override;

	// Entity: renders itself
	virtual void render(Renderer* renderer) override;

	// InputContext: handles user input
	virtual bool onNotify(const KeyboardInput& key) override;
};


// 
class PipeEntity : public Entity {
private:
	StaticPhysics*	_physics = nullptr;
	Sprite*	_top = nullptr;
	Sprite* _mid = nullptr;
	Rect	_topPos;
	Rect	_midPos;
	bool	_upward;

public:
	PipeEntity(StaticPhysics* physics, Sprite* top, Sprite* mid, bool upward = true);
	virtual ~PipeEntity();

	virtual void update() override;

	virtual void render(Renderer* renderer) override;
};


/*
	DEPipeEntity 

	was trying to make a pipe, but this is actually a double-ended pipe, 
	and the ones in the game are supposed to be single-ended
	maybe it'll be useful sometime
*/
class DEPipeEntity : public Entity {
private:
	StaticPhysics*	_physics = nullptr;
	Sprite*	_top = nullptr;
	Sprite*	_mid = nullptr;
	Sprite*	_bot = nullptr;
	Rect	_topPos;
	Rect	_midPos;
	Rect	_botPos;

public:
	DEPipeEntity(StaticPhysics* physics, Sprite* bot, Sprite* mid, Sprite* top);
	virtual ~DEPipeEntity();

	virtual void update() override;

	virtual void render(Renderer* renderer) override;
};