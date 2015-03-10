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
PhysicsEngine::~PhysicsEngine()
{
	while (dEntities.size() > 0)
	{
		DynamicPhysics *dynamicPhysics = dEntities.back();
		dEntities.pop_back();
		delete dynamicPhysics;
	}
	while (sEntities.size() > 0)
	{
		StaticPhysics *staticPhysics = sEntities.back();
		sEntities.pop_back();
		delete staticPhysics;
	}
}

void PhysicsEngine::addDynamicPhysics(DynamicPhysics *dynamicPhysics)
{
	dEntities.push_back(dynamicPhysics);
}

void PhysicsEngine::addStaticPhysics(StaticPhysics *staticPhysics)
{
	sEntities.push_back(staticPhysics);
}

void PhysicsEngine::update(std::vector<Entity*> entities)
{
	for (auto it : entities)
	{
		it->_physics->update(0);
	}
	/*for (auto it : sEntities)
	{
		it->update(0);
	}*/
}

bool PhysicsEngine::deletePhysics(StaticPhysics *staticPhysics)
{
	std::list<StaticPhysics*>::iterator found = std::find(sEntities.begin(), sEntities.end(), staticPhysics);

	if (found != sEntities.end())
	{
		StaticPhysics *sPhysics = *found;
		sEntities.erase(found);
		delete sPhysics;
		sPhysics = nullptr;
		return true;
	}
	return false;
}

bool PhysicsEngine::deletePhysics(DynamicPhysics *dynamicPhysics)
{
	std::list<DynamicPhysics*>::iterator found = std::find(dEntities.begin(), dEntities.end(), dynamicPhysics);

	if (found != dEntities.end())
	{
		DynamicPhysics *dPhysics = *found;
		dEntities.erase(found);
		delete dPhysics;
		dPhysics = nullptr;
		return true;
	}
	return false;
}


void PhysicsEngine::setResolver(Resolver *res)
{
	delete resolver;
	resolver = res;
}

void PhysicsEngine::detectColissions(std::vector<Entity*> dynamicEntities, std::vector<Entity*> staticEntities)
{

	if (dynamicEntities.size() > 0 && staticEntities.size() > 0)
	{
		float dynamicPhysicsXpos, dynamicPhysicsYpos, dynamicPhysicsH, dynamicPhysicsW;
		float staticPhysicsXpos, staticPhysicsYpos, staticPhysicsH, staticPhysicsW;
		for (auto dynamicEntity : dynamicEntities)
		{
			dynamicPhysicsXpos = dynamicEntity->_physics->getXPosition();
			dynamicPhysicsYpos = dynamicEntity->_physics->getYPosition();
			dynamicPhysicsH = dynamicEntity->_physics->getHeight();
			dynamicPhysicsW = dynamicEntity->_physics->getWidth();
			for (auto staticEntity : staticEntities)
			{
				staticPhysicsXpos = staticEntity->_physics->getXPosition();
				staticPhysicsYpos = staticEntity->_physics->getYPosition();
				staticPhysicsH = staticEntity->_physics->getHeight();
				staticPhysicsW = staticEntity->_physics->getWidth();
				if (dynamicPhysicsYpos + dynamicPhysicsH < staticPhysicsYpos || dynamicPhysicsYpos > staticPhysicsYpos + staticPhysicsH)
				{
				}
				else if (dynamicPhysicsXpos + dynamicPhysicsW < staticPhysicsXpos || dynamicPhysicsXpos > staticPhysicsXpos + staticPhysicsW)
				{
				}
				else
				{
					collision(dynamicEntity->_physics, staticEntity->_physics);
				}

			}
		}
	}

}

void PhysicsEngine::detectColissions(std::vector<Entity*> entities)
{
	/*
	Collision between dynamic objects
	to be implemented later...
	*/
	list<DynamicPhysics*>::iterator first, second, firstEnd, secondEnd;
	second = first = dEntities.begin();
	firstEnd = dEntities.end();
	advance(second, 1);
	advance(firstEnd, -1);
	if (dEntities.size() > 1)
	{
		for (auto first = dEntities.begin(); first != firstEnd; first++)
			for (second; second != dEntities.end(); second++)
			{

			}

	}

	
}

// check if a point intersects
inline bool pointIntersect(float pt, float lowest, float highest) {
	return pt > lowest && pt < highest;
}

// check if a range intersects partially
inline bool partialIntersect(float leftLow, float leftHigh, float rightLow, float rightHigh) {
	bool right, left;
	left = pointIntersect(leftLow, rightLow, rightHigh);
	right = pointIntersect(leftHigh, rightLow, rightHigh);
	return right != left; // one intersects
}

// check if a range intersects fully
inline bool fullIntersect(float leftLow, float leftHigh, float rightLow, float rightHigh) {

	return pointIntersect(leftLow, rightLow, rightHigh) && pointIntersect(leftHigh, rightLow, rightHigh); // both intersect
}


void PhysicsEngine::collision(Physics *dynamicPhysics, Physics *pe)
{
	bool left = false, right = false, top = false, bottom = false;
	/*float
		dynamicPhysicsXpos = dynamicPhysics->getXPosition(), 
		dynamicPhysicsYpos = dynamicPhysics->getYPosition(), 
		dynamicPhysicsH = dynamicPhysics->getHeight(), 
		dynamicPhysicsW = dynamicPhysics->getWidth();
	float 
		peXpos = pe->getXPosition(),
		peYpos = pe->getYPosition(), 
		peH = pe->getHeight(), 
		peW = pe->getWidth();

	
	//Check collision
	if ((dynamicPhysicsYpos + dynamicPhysicsH) >= peYpos 
		&& dynamicPhysicsYpos < peYpos)
		bottom = true;
	if (dynamicPhysicsYpos <= (peYpos + peH) 
		&& dynamicPhysicsYpos > peYpos)
		top = true;
	if (dynamicPhysicsXpos <= peXpos + peW 
		&& dynamicPhysicsXpos > peXpos)
		left = true;
	if ((dynamicPhysicsXpos + dynamicPhysicsW) >= peXpos 
		&& dynamicPhysicsXpos < peXpos)
		right = true;
	*/
	// first physics object
	float dpXlow = dynamicPhysics->getXPosition();
	float dpXhigh = dpXlow + dynamicPhysics->getWidth();
	float dpYlow = dynamicPhysics->getYPosition();
	float dpYhigh = dpYlow + dynamicPhysics->getHeight();

	// second physics object
	float pXlow = pe->getXPosition();
	float pXhigh = pXlow + pe->getWidth();
	float pYlow = pe->getYPosition();
	float pYhigh = pYlow + pe->getHeight();


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



	setResolver(rFactory.createResolver(top, bottom, left, right));

	if (resolver != nullptr)
		resolver->resolve(dynamicPhysics, pe);
}
