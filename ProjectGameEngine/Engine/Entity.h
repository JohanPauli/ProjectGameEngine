#pragma once



class Graphics;
class Input;
class Physics;
class Renderer;


/*
	General component-based Entity class
	
	graphics and input components depend on a physics component existing for x,y, and acceleration
	input component should be unregistered from the InputMapper before deleting the entity
*/
class Entity {
private:
	Graphics*	graphics;

	// physics is public so Input and Graphics have access to x, y, and velocity
public:
	Input*		input;
	Physics*	physics;

public:
	Entity(Physics* physics, Graphics* graphics = nullptr, Input* input = nullptr);
	virtual ~Entity();

	// update components' logic
	virtual void update();

	// render entity
	virtual void render(Renderer* renderer);
};