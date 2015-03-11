#pragma once

#include "Timer.h"
#include "Rect.h"

// forward declarations
class Entity;
class Renderer;
class Sprite;
class SpriteSheet;
class Physics;



// base class
class Graphics {
public:
	virtual ~Graphics() {}

	// update any logic inside the graphics component
	virtual void update(Entity& entity) = 0;
	// render the graphics object
	virtual void render(Entity& entity, Renderer* renderer) = 0;
};






// concrete classes

// player/bird 

class BirdGraphics : public Graphics {
private:
	UpdateTimer	 _flapTimer;
	SpriteSheet* _spriteSheet;
public:
	BirdGraphics(SpriteSheet* spriteSheet, UpdateTimer timer);
	virtual ~BirdGraphics();
	virtual void update(Entity& entity) override;
	virtual void render(Entity& entity, Renderer* renderer) override;
};


// pipe

class PipeGraphics : public Graphics {
private:
	Sprite* _top;
	Sprite* _mid;
	Rect	_topPos;
	Rect	_midPos;
	bool	_upward;
private:
	// calculate the positions of both sprites
	void calcPos(const Entity& entity);
public:
	PipeGraphics(Sprite* top, Sprite* mid, bool upward = true);
	virtual ~PipeGraphics();
	virtual void update(Entity& entity) override;
	virtual void render(Entity& entity, Renderer* renderer) override;
};

// Background

class BackgroundGraphics : public Graphics{
private:
	Sprite* _sky;
	Sprite* _land;
	Rect skyPos;
	Rect landPos;

public:
	BackgroundGraphics(Sprite* sky, Sprite* land);
	virtual ~BackgroundGraphics();
	virtual void update(Entity& entity) override;
	virtual void render(Entity& entity, Renderer* renderer) override;
};