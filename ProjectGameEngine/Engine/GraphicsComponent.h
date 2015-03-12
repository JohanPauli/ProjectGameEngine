#pragma once

#include "Timer.h"
#include "Rect.h"

#include <vector>

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
	Sprite* background;
	Rect pos;

public:
	BackgroundGraphics(Sprite* sprite);
	virtual ~BackgroundGraphics();
	virtual void update(Entity& entity) override;
	virtual void render(Entity& entity, Renderer* renderer) override;
};

// looping background
class BackgroundGraphics2 : public Graphics {
private:
	Sprite* _background;
	int		_bgWidth;
	int		_bgHeight;
	float	_scale;
	int		_slowDown;

public:
	BackgroundGraphics2(Sprite* sprite, float scale = 4, int slowDown = 4);
	virtual ~BackgroundGraphics2();
	virtual void update(Entity& entity) override;
	virtual void render(Entity& entity, Renderer* renderer) override;
};

class ForegroundGraphics : public Graphics {
private:
	Sprite* _background;
	int		_bgWidth;
	int		_bgHeight;
	float	_scale;
	int		_slowDown;
	int		_numberOfRenders;

public:
	ForegroundGraphics(Sprite* sprite, float scale = 4, int slowDown = 4);
	virtual ~ForegroundGraphics();
	virtual void update(Entity& entity) override;
	virtual void render(Entity& entity, Renderer* renderer) override;
};