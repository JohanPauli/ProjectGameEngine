#pragma once



class Graphics;
class Input;
class InputContext;
class Physics;
class Renderer;
class Rect;

/*
Eg havi gjørt hendan fyri skilja graphics og Entity frá hvørjum øðrum...
eitt sindur hacky, so tað verðrur kanska broytt seinni
*/
class EntityInterface
{
public:
	virtual void update() = 0;
	virtual void render(Renderer* renderer) = 0;
};

/*
	General component-based Entity class
	
	graphics and input components depend on a physics component existing for x,y, and acceleration
	input component should be unregistered from the InputMapper before deleting the entity
*/
class Entity {
	friend class PhysicsEngine;
private:
	Graphics*	graphics;
	Input*		input;
	Physics*	physics;

public:
	Entity(Physics* physics, Graphics* graphics = nullptr, Input* input = nullptr);
	virtual ~Entity();

	// update components' logic
	virtual void update();

	// render entity
	virtual void render(Renderer* renderer);

	// accessors
	Rect	getRect() const;
	double	getAngle() const;
	int		getX() const;
	int		getY() const;
	int		getWidth() const;
	int		getHeight() const;
	float	getXvelocity() const;
	float	getYvelocity() const;
	void	setXvelocity(float vel) const;
	void	setYvelocity(float vel) const;
	InputContext* getInputContext() const;
};