#pragma once

#include "Rendering.h"


// entity interface
/*class Entity {
public:
	virtual void update() = 0;			// update logic
	virtual void render(Renderer*) = 0;	// render the entity
};
*/

class Graphics;
class Input;
class Physics;



class Entity {
private:
	Graphics*	_graphics;
	Input*		_input;

	// physics is public so Input and Graphics have access to x, y, and velocity
public:
	Physics*	_physics;

public:
	Entity(Graphics* graphics, Physics* physics = nullptr, Input* input = nullptr);
	virtual ~Entity();
	virtual void update();
	virtual void render(Renderer* renderer);
};