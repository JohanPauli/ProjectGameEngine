#include "Physics.h"

#include "Rect.h"


void Physics::update(int number)
{
	//xVelocity += xAcceleration;
	_yVelocity += _yAcceleration;
	_xPosition += _xVelocity;
	_yPosition += _yVelocity;
}

DynamicPhysics::DynamicPhysics(float xAcceleration, float yAcceleration, float xVelocity, float yVelocity, float xPosition, float yPosition, float height, float width) 
: Physics(xAcceleration, yAcceleration, xVelocity, yVelocity, xPosition, yPosition, height, width)
{
	/*
	In DynamicPhysics the physics box is smaller than the
	drawing box, this is to minimize the chance of collision
	outside the sprite, when it is rotated
	*/

	//Maybe it would be better to pass the rectWidt and rectHeight
	//in the constructor, and calculate the physics box from these
	rectWidth = (int)(width * 1.25f);
	rectHeight = (int)(height * 1.25f);

	int xMidPoint = (int)(xPosition + (width / 2));
	int yMidPoint = (int)(yPosition + (height / 2));

	rectXpos = xMidPoint - (int)(rectWidth / 2);
	rectYpos = yMidPoint - (int)(rectHeight / 2);
}


void DynamicPhysics::update(int number)
{
	Physics::update(number);

	int xMidPoint = (int)(_xPosition + (_width / 2));
	int yMidPoint = (int)(_yPosition + (_height / 2));

	rectXpos = (int)(xMidPoint - (rectWidth / 2));
	rectYpos = (int)(yMidPoint - (rectHeight / 2));
}

Rect DynamicPhysics::getRect()
{
	return Rect(rectXpos, rectYpos, rectWidth, rectHeight);
}

void DynamicPhysics::resolveCollision(float collVelocity, Side side)
{
	switch (side)
	{
	case Side::TOP:
		setYVelocity(collVelocity);
		break;
	case Side::BOTTOM:
		setYVelocity(collVelocity*0.4f);
		break;
	case Side::LEFT:
		setXVelocity(collVelocity);
		break;
	case Side::RIGHT:
		setXVelocity(collVelocity);
		break;
	}
}

void StaticPhysics::update(int number)
{
	Physics::update(number);
}

Rect StaticPhysics::getRect()
{
	return Rect((int)_xPosition, (int)_yPosition, (int)_width, (int)_height);
}

/*
Physics engine__
*/
PhysicsEngine::~PhysicsEngine() {
	delete resolver;
}

void PhysicsEngine::setResolver(Resolver *res)
{
	delete resolver;
	resolver = res;
}



// check if a point intersects a range
inline bool pointIntersect(float pt, float lowest, float highest) {
	return pt > lowest && pt < highest;
}

// check if a range another range intersects fully
inline bool fullIntersect(float leftLow, float leftHigh, float rightLow, float rightHigh) {
	return (pointIntersect(leftLow, rightLow, rightHigh) && pointIntersect(leftHigh, rightLow, rightHigh)); // both intersect
}


void PhysicsEngine::detectCollisions(const std::deque<Entity*>& dynamicEntities, const std::deque<Entity*>& staticEntities)
{
	for (auto dynamicEntity : dynamicEntities)
	{
		detectCollisions(dynamicEntity, staticEntities);
	}
}

bool PhysicsEngine::detectCollisions(Entity* entity, const std::deque<Entity*>& entities2) {
	bool collisionFlag = false;
	for (auto ent2 : entities2) {
		if (detectCollisions(entity, ent2))
			collisionFlag = true;
	}
	return collisionFlag;
}
bool PhysicsEngine::detectCollisions(Entity* entity, Entity* entity2) {
	Physics* phys = entity->physics;
	Physics* phys2 = entity2->physics;

	// first physics object
	float dpXlow = phys->getXPosition();
	float dpXhigh = dpXlow + phys->getWidth();
	float dpYlow = phys->getYPosition();
	float dpYhigh = dpYlow + phys->getHeight();

	// second physics object
	float pXlow = phys2->getXPosition();
	float pXhigh = pXlow + phys2->getWidth();
	float pYlow = phys2->getYPosition();
	float pYhigh = pYlow + phys2->getHeight();

	if (dpYhigh < pYlow || dpYlow > pYhigh) {}		// entity is below or above entity2
	else if (dpXhigh < pXlow || dpXlow > pXhigh) {} // entity is to the left or right of entity2
	else {
		// collisions in flappy bird all result in game ending, dont need to detect where collision occured

		//collision(phys, phys2);
		return true;
	}
	return false;
}


void PhysicsEngine::collision(Physics *phys, Physics *phys2)
{
	bool left = false, right = false, top = false, bottom = false;

	// first physics object
	float dpXlow = phys->getXPosition();
	float dpXhigh = dpXlow + phys->getWidth();
	float dpYlow = phys->getYPosition();
	float dpYhigh = dpYlow + phys->getHeight();

	// second physics object
	float pXlow = phys2->getXPosition();
	float pXhigh = pXlow + phys2->getWidth();
	float pYlow = phys2->getYPosition();
	float pYhigh = pYlow + phys2->getHeight();


	// assume it isn't possible for full Y-intersection at the same time as full X-intersection for simplicity
	// check for full intersections first
	if (fullIntersect(dpXlow, dpXhigh, pXlow, pXhigh)) {
		// check if there's a Y intersection. 
		if (pointIntersect(dpYlow, pYlow, pYhigh))
			top = true;
		else if (pointIntersect(dpYhigh, pYlow, pYhigh))
			bottom = true;
	}
	else if (fullIntersect(dpYlow, dpYhigh, pYlow, pYhigh)) {
		// check if there's an X intersection. 
		if (pointIntersect(dpXlow, pXlow, pXhigh))
			left = true;
		else if (pointIntersect(dpXhigh, pXlow, pXhigh))
			right = true;
	}
	// check for partial intersections
	else {
		// X intersects
		if (pointIntersect(dpXlow, pXlow, pXhigh))
			left = true;
		else if (pointIntersect(dpXhigh, pXlow, pXhigh))
			right = true;
		// Y intersects
		if (pointIntersect(dpYlow, pYlow, pYhigh))
			top = true;
		else if (pointIntersect(dpYhigh, pYlow, pYhigh))
			bottom = true;
	}

	// check if collision occured
	if (top || bottom || left || right) {
		setResolver(rFactory.createResolver(top, bottom, left, right));
		if (resolver != nullptr)
			resolver->resolve(phys, phys2);
	}
}
