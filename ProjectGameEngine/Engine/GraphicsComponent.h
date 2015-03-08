#pragma once

#include "Timer.h"
#include "Rect.h"

// forward declarations
class Entity;
class Renderer;
class Sprite;
class SpriteSheet;



// base class
class Graphics {
public:
	virtual ~Graphics() {}
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
	const static long MS_PER_FRAME = 250;
public:
	BirdGraphics(SpriteSheet* spriteSheet, Timer* timer);
	virtual ~BirdGraphics();
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
	bool	_isInit = false;
private:
	// calculate the positions of both sprites
	void init(Entity& entity);
public:
	PipeGraphics(Sprite* top, Sprite* mid, bool upward = true);
	virtual ~PipeGraphics();
	virtual void render(Entity& entity, Renderer* renderer) override;
};