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

void PhysicsEngine::update()
{
	for (auto it : dEntities)
	{
		it->update(0);
	}
	for (auto it : sEntities)
	{
		it->update(0);
	}
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

void PhysicsEngine::detectColissions()
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


	if (dEntities.size() > 0 && sEntities.size() > 0)
	{
		float dynamicPhysicsXpos, dynamicPhysicsYpos, dynamicPhysicsH, dynamicPhysicsW;
		float staticPhysicsXpos, staticPhysicsYpos, staticPhysicsH, staticPhysicsW;
		for (auto dynamicPhysics : dEntities)
		{
			dynamicPhysicsXpos = dynamicPhysics->getXPosition();
			dynamicPhysicsYpos = dynamicPhysics->getYPosition();
			dynamicPhysicsH = dynamicPhysics->getHeight();
			dynamicPhysicsW = dynamicPhysics->getWidth();
			for (auto staticPhysics : sEntities)
			{
				staticPhysicsXpos = staticPhysics->getXPosition();
				staticPhysicsYpos = staticPhysics->getYPosition();
				staticPhysicsH = staticPhysics->getHeight();
				staticPhysicsW = staticPhysics->getWidth();
				if (dynamicPhysicsYpos + dynamicPhysicsH < staticPhysicsYpos || dynamicPhysicsYpos > staticPhysicsYpos + staticPhysicsH)
				{
				}
				else if (dynamicPhysicsXpos + dynamicPhysicsW < staticPhysicsXpos || dynamicPhysicsXpos > staticPhysicsXpos + staticPhysicsW)
				{
				}
				else
				{
					collision(dynamicPhysics, staticPhysics);
				}

			}
		}
	}

}

void PhysicsEngine::collision(DynamicPhysics *dynamicPhysics, Physics *pe)
{
	bool left = false, right = false, top = false, bottom = false;
	float dynamicPhysicsXpos = dynamicPhysics->getXPosition(), dynamicPhysicsYpos = dynamicPhysics->getYPosition(), dynamicPhysicsH = dynamicPhysics->getHeight(), dynamicPhysicsW = dynamicPhysics->getWidth();
	float peXpos = pe->getXPosition(), peYpos = pe->getYPosition(), peH = pe->getHeight(), peW = pe->getWidth();
	//Check corner collision


	if ((dynamicPhysicsYpos + dynamicPhysicsH) >= peYpos && dynamicPhysicsYpos < peYpos)
		bottom = true;
	else if (dynamicPhysicsYpos <= (peYpos + peH) && dynamicPhysicsYpos < peYpos)
		top = true;
	else if (dynamicPhysicsXpos <= peXpos + peW && dynamicPhysicsXpos > peXpos)
		left = true;
	else if (dynamicPhysicsXpos + dynamicPhysicsW >= peXpos && dynamicPhysicsXpos < peXpos)
		right = true;

	setResolver(rFactory.createResolver(top, bottom, left, right));

	if (resolver != nullptr)
		resolver->resolve(dynamicPhysics, pe->getXVelocity(), pe->getYVelocity());
}
