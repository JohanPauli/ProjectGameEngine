#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include "Resolver.h"
#include "Rect.h"

using std::vector;

enum SIDES { TOP, BOTTOM, LEFT, RIGHT };

//General physics entity
class Physics
{
protected:
	float xAcceleration;
	float yAcceleration;

	float xVelocity;
	float yVelocity;

	float xPosition;
	float yPosition;

	float height;
	float width;
public:
	//Constructor
	Physics(float _xAcceleration, float _yAcceleration, float _xVelocity, float _yVelocity, float _xPosition, float _yPosition, float _height, float _width) :
		xAcceleration(_xAcceleration), yAcceleration(_yAcceleration), xVelocity(_yAcceleration), yVelocity(_yVelocity),
		xPosition(_xPosition), yPosition(_yPosition), height(_height), width(_width) {}
	virtual ~Physics() {}

	//getters
	float getXAcceleration() const { return xAcceleration; }
	float getYAcceleration() const { return yAcceleration; }
	float getXVelocity() const { return xVelocity; }
	float getYVelocity() const { return yVelocity; }
	float getXPosition() const { return xPosition; }
	float getYPosition() const { return yPosition; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }

	//setters
	void setXAcceleration(float _xAcceleration) { xAcceleration = _xAcceleration; }
	void setYAcceleration(float _yAcceleration) { yAcceleration = _yAcceleration; }
	void setXVelocity(float _xVelocity)  { xVelocity = _xVelocity; }
	void setYVelocity(float _yVelocity) { yVelocity = _yVelocity; }
	void setXPosition(float _xPosition) { xPosition = _xPosition; }
	void setYPosition(float _yPosition) { yPosition = _yPosition; }

	//Pure virtual update function
	virtual void update(int elapsed) = 0;

	//getRect will have different implementation
	//for static and dynamic physics
	virtual Rect getRect() = 0;


};
//Dynamic entity used by moving objects in the game
class DynamicPhysics : public Physics
{
private:
	int rectWidth;
	int rectHeight;
	int rectXpos;
	int rectYpos;
public:
	DynamicPhysics(float _xAcceleration, float _yAcceleration, float _xVelocity, float _yVelocity, float _xPosition, float _yPosition, float _height, float _width);
	void jump();
	virtual void update(int elapsed);
	virtual Rect getRect();
	void resolveCollision(float collVelocity, SIDES);

};

//Static entity used by  non moving objects in the game
class StaticPhysics : public Physics
{
public:
	StaticPhysics(float _xAcceleration, float _yAcceleration, float _xVelocity, float _yVelocity, float _xPosition, float _yPosition, float _height, float _width) : Physics(_xAcceleration, _yAcceleration, _xVelocity, _yVelocity, _xPosition, _yPosition, _height, _width){}
	virtual void update(int elapsed);
	virtual Rect getRect();
};

//Resolver class is used to resolve collision between objects
class Resolver;


//The physics engine updates the physicts objects, and detects collisions
class PhysicsEngine
{
public:
	~PhysicsEngine();
	void update();
	void detectColissions();
	void collision(DynamicPhysics *, Physics *);
	void addDynamicPhysics(DynamicPhysics *de);
	void addStaticPhysics(StaticPhysics *se);
	void setResolver(Resolver *res);

private:
	vector<DynamicPhysics*> dEntities;
	vector<StaticPhysics*> sEntities;
	ResolverFactory rFactory;
	Resolver *resolver;

};


#endif