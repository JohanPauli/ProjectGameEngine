#include "Resolver.h"
#include "Physics.h"


// logic was buggy and I don't know how to fix it. going with something simpler for now

inline void resTop(Physics* physics, Physics* otherPhysics) {
	float leftYV = physics->getYVelocity();

	// physics moving towards otherPhysics
	if (leftYV < 0)
		leftYV = -leftYV;

	// set new value
	physics->setYVelocity(leftYV);
}


inline void resBottom(Physics* physics, Physics* otherPhysics) {
	float leftYV = physics->getYVelocity();

	// physics moving towards otherPhysics
	if (leftYV > 0)
		leftYV = -leftYV;

	// set new value
	physics->setYVelocity(leftYV);
}

inline void resLeft(Physics* physics, Physics* otherPhysics) {
	float leftXV = physics->getXVelocity();

	// physics moving towards otherPhysics
	if (leftXV < 0)
		leftXV = -leftXV;

	// set new value
	physics->setXVelocity(leftXV);
}

inline void resRight(Physics* physics, Physics* otherPhysics) {
	float leftXV = physics->getXVelocity();

	// physics moving towards otherPhysics
	if (leftXV > 0)
		leftXV = -leftXV;
	 
	// set new value
	physics->setXVelocity(leftXV);
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