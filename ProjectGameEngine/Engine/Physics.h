#pragma once

#include <list>
#include <algorithm>
#include <cmath>
#include <vector>
#include <deque>
#include "Resolver.h"
#include "Entity.h"

using std::list;

// forward declarations
class Rect;

enum class Side { TOP, BOTTOM, LEFT, RIGHT };

// math constants
static const float PI = 3.14159265358979323846264338327950f;


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
		_xAcceleration(xAcceleration), _yAcceleration(yAcceleration), _xVelocity(xVelocity), _yVelocity(yVelocity),
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

	// angle of the object
	double getAngle() { return atan2(_yVelocity, _xVelocity) / (PI / 180); }

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
	virtual void update(int elapsed) override;
	virtual Rect getRect() override;
	void resolveCollision(float collVelocity, Side);

};

//Static entity used by  non moving objects in the game
class StaticPhysics : public Physics
{
public:
	StaticPhysics(float xAcceleration, float yAcceleration, float xVelocity, float yVelocity, float xPosition, float yPosition, float height, float width) 
		: Physics(xAcceleration, yAcceleration, xVelocity, yVelocity, xPosition, yPosition, height, width){}
	virtual void update(int elapsed) override;
	virtual Rect getRect() override;
};



//The physics engine updates the physicts objects, and detects collisions
class PhysicsEngine
{
public:
	~PhysicsEngine();
	//void update(std::vector<Entity*> entities);
	void detectCollisions(const std::deque<Entity*>& entities1, const std::deque<Entity*>& entities2);
	bool detectCollisions(Entity* entity, const std::deque<Entity*>& entities2);
	bool detectCollisions(Entity* entity, Entity* entity2);

	void collision(Physics* phy, Physics* phy2);
	void setResolver(Resolver *res);

private:
	ResolverFactory rFactory;
	Resolver *resolver;

};