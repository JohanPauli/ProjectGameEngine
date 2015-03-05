#pragma once

#include <list>
#include <algorithm>

#include "Resolver.h"

using std::list;

// forward declarations
class Rect;


enum class Side { TOP, BOTTOM, LEFT, RIGHT };

//General physics entity
class Physics
{
protected:
	float _xAcceleration;
	float _yAcceleration;

	float _xVelocity;
	float _yVelocity;

	float _xPosition;
	float _yPosition;

	float _height;
	float _width;
public:
	//Constructor
	Physics(float xAcceleration, float yAcceleration, float xVelocity, float yVelocity, float xPosition, float yPosition, float height, float width) :
		_xAcceleration(xAcceleration), _yAcceleration(yAcceleration), _xVelocity(yAcceleration), _yVelocity(yVelocity),
		_xPosition(xPosition), _yPosition(yPosition), _height(height), _width(width) {}
	virtual ~Physics() {}

	//getters
	float getXAcceleration() const { return _xAcceleration; }
	float getYAcceleration() const { return _yAcceleration; }
	float getXVelocity() const { return _xVelocity; }
	float getYVelocity() const { return _yVelocity; }
	float getXPosition() const { return _xPosition; }
	float getYPosition() const { return _yPosition; }
	float getWidth() const { return _width; }
	float getHeight() const { return _height; }

	//setters
	void setXAcceleration(float xAcceleration) { _xAcceleration = xAcceleration; }
	void setYAcceleration(float yAcceleration) { _yAcceleration = yAcceleration; }
	void setXVelocity(float xVelocity)  { _xVelocity = xVelocity; }
	void setYVelocity(float yVelocity) { _yVelocity = yVelocity; }
	void setXPosition(float xPosition) { _xPosition = xPosition; }
	void setYPosition(float yPosition) { _yPosition = yPosition; }

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
	DynamicPhysics(float xAcceleration, float yAcceleration, float xVelocity, float yVelocity, float xPosition, float yPosition, float height, float width);
	virtual void update(int elapsed);
	virtual Rect getRect();
	void resolveCollision(float collVelocity, Side);

};

//Static entity used by  non moving objects in the game
class StaticPhysics : public Physics
{
public:
	StaticPhysics(float xAcceleration, float yAcceleration, float xVelocity, float yVelocity, float xPosition, float yPosition, float height, float width) : Physics(xAcceleration, yAcceleration, xVelocity, yVelocity, xPosition, yPosition, height, width){}
	virtual void update(int elapsed);
	virtual Rect getRect();
};



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

	/*
	Used when a single physics element has to be deleted
	*/
	bool deletePhysics(StaticPhysics *staticPhysics);
	bool deletePhysics(DynamicPhysics *dynamicPhysics);

private:
	list<DynamicPhysics*> dEntities;
	list<StaticPhysics*> sEntities;
	ResolverFactory rFactory;
	Resolver *resolver;

};