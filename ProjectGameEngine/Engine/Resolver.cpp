#include "Resolver.h"
#include "Physics.h"


// logic was buggy and I don't know how to fix it. going with something simpler for now

inline void resTop(Physics* physics, Physics* otherPhysics) {
	physics->setYVelocity(-physics->getYVelocity());
	/*
	float leftYV = physics->getYVelocity();
	float rightYV = otherPhysics->getYVelocity();
	float newYVel;

	// logic
	if (rightYV > 0 && leftYV > 0)
		newYVel = rightYV + leftYV;
	else if (rightYV < 0 && leftYV < 0)
		newYVel = rightYV - leftYV;
	else
		newYVel = -(leftYV - rightYV);

	// set new value
	physics->setYVelocity(newYVel);
	*/
}


inline void resBottom(Physics* physics, Physics* otherPhysics) {
	physics->setYVelocity(-physics->getYVelocity());
	/*
	float leftYV = physics->getYVelocity();
	float rightYV = otherPhysics->getYVelocity();
	float newYVel;

	// logic
	if (rightYV < 0 && leftYV < 0)
		newYVel = rightYV + leftYV;
	else if (rightYV > 0 && leftYV > 0)
		newYVel = rightYV - leftYV;
	else
		newYVel = -(leftYV - rightYV);

	// set new value
	physics->setYVelocity(newYVel);
	*/
}

inline void resLeft(Physics* physics, Physics* otherPhysics) {
	physics->setXVelocity(physics->getXVelocity());
	/*
	float leftXV = physics->getXVelocity();
	float rightXV = otherPhysics->getXVelocity();
	float newXVel;

	// logic
	if (rightXV <= 0 && leftXV < 0)		// both moving left
		newXVel = rightXV - leftXV;
	else if (rightXV < 0 && leftXV < 0)	// both moving right
		newXVel = rightXV + leftXV;
	else								// moving in opposite directions
		newXVel = -(leftXV - rightXV);

	// set new value
	physics->setXVelocity(newXVel);
	*/
}

inline void resRight(Physics* physics, Physics* otherPhysics) {
	physics->setXVelocity(physics->getXVelocity());
	/*
	float leftXV = physics->getXVelocity();
	float rightXV = otherPhysics->getXVelocity();
	float newXVel;

	// logic
	newXVel = leftXV;
	if (rightXV > 0 && leftXV > 0)
		newXVel = rightXV - leftXV;
	else if (rightXV< 0 && leftXV < 0)
		newXVel = rightXV + leftXV;
	else
		newXVel = -(leftXV - rightXV);

	// set new value
	physics->setXVelocity(newXVel);
	*/
}


/*
Resolvers
*/

//The factory
Resolver* ResolverFactory::createResolver(bool top, bool bottom, bool left, bool right)
{
	if (top && left)
		return new TopLeftResolver();
	else if (top && right)
		return new TopRightResolver();
	else if (bottom && left)
		return new BottomLeftResolver();
	else if (bottom && right)
		return new BottomRightResolver();
	else if (left)
		return new LeftResolver();
	else if (right)
		return new RightResolver();
	else if (top)
		return new TopResolver();
	else if (bottom)
		return new BottomResolver();
	else
		return nullptr;
}

//Resolvers
void TopLeftResolver::resolve(Physics *physics, Physics *otherPhysics)
{
	resTop(physics, otherPhysics);
	resLeft(physics, otherPhysics);
}

void TopRightResolver::resolve(Physics *physics, Physics *otherPhysics)
{

	resTop(physics, otherPhysics);
	resRight(physics, otherPhysics);
}

void BottomLeftResolver::resolve(Physics *physics, Physics *otherPhysics)
{
	resBottom(physics, otherPhysics);
	resLeft(physics, otherPhysics);
}

void BottomRightResolver::resolve(Physics *physics, Physics *otherPhysics)
{
	resBottom(physics, otherPhysics);
	resRight(physics, otherPhysics);
}

void LeftResolver::resolve(Physics *physics, Physics *otherPhysics)
{
	resLeft(physics, otherPhysics);
}

void RightResolver::resolve(Physics *physics, Physics *otherPhysics)
{
	resRight(physics, otherPhysics);
}

void TopResolver::resolve(Physics *physics, Physics *otherPhysics)
{
	resTop(physics, otherPhysics);
}

void BottomResolver::resolve(Physics *physics, Physics *otherPhysics)
{
	resBottom(physics, otherPhysics);
	//if (physics->getYVelocity() == 0)
	//	physics->setYAcceleration(0);
}