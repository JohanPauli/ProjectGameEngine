#include "Physics.h"

void Physics::update(int number)
{
	//xVelocity += xAcceleration;
	yVelocity += yAcceleration;
	xPosition += xVelocity;
	yPosition += yVelocity;
}

DynamicPhysics::DynamicPhysics(float _xAcceleration, float _yAcceleration, float _xVelocity, float _yVelocity, float _xPosition, float _yPosition, float _height, float _width) : Physics(_xAcceleration, _yAcceleration, _xVelocity, _yVelocity, _xPosition, _yPosition, _height, _width)
{
	/*
	In DynamicPhysics the physics box is smaller than the
	drawing box, this is to minimize the chance of collision
	outside the sprite, when it is rotated
	*/

	//Maybe it would be better to pass the rectWidt and rectHeight
	//in the constructor, and calculate the physics box from these
	rectWidth = width * 1.25;
	rectHeight = height * 1.25;

	int xMidPoint = xPosition + (width / 2);
	int yMidPoint = yPosition + (height / 2);

	rectXpos = xMidPoint - (rectWidth / 2);
	rectYpos = yMidPoint - (rectHeight / 2);
}


void DynamicPhysics::jump()
{
	yVelocity = -15;
}

void DynamicPhysics::update(int number)
{
	Physics::update(number);

	int xMidPoint = xPosition + (width / 2);
	int yMidPoint = yPosition + (height / 2);

	rectXpos = xMidPoint - (rectWidth / 2);
	rectYpos = yMidPoint - (rectHeight / 2);
}

Rect DynamicPhysics::getRect()
{
	return Rect(rectXpos, rectYpos, rectWidth, rectHeight);
}

void DynamicPhysics::resolveCollision(float collVelocity, SIDES side)
{
	switch (side)
	{
	case TOP:
		setYVelocity(collVelocity);
		break;
	case BOTTOM:
		setYVelocity(collVelocity*0.4);
		break;
	case LEFT:
		setXVelocity(collVelocity);
		break;
	case RIGHT:
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
	return Rect(xPosition, yPosition, width, height);
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

void PhysicsEngine::setResolver(Resolver *res)
{
	delete resolver;
	resolver = res;
}

void PhysicsEngine::detectColissions()
{
	if (dEntities.size() > 1)
	{
		for (auto first = dEntities.begin(); first != (dEntities.end() - 1); first++)
			for (auto second = first + 1; second != dEntities.end(); second++)
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
