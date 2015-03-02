#include "Physics.h"

void PhysicsEntity::update(int number)
{
	//xVelocity += xAcceleration;
	yVelocity += yAcceleration;
	xPosition += xVelocity;
	yPosition += yVelocity;
}

void DynamicEntity::jump()
{
	yVelocity = -15;
}

void DynamicEntity::update(int number)
{
	PhysicsEntity::update(number);
}

void DynamicEntity::resolveCollision(int collVelocity, SIDES side)
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

void StaticEntity::update(int number)
{
	PhysicsEntity::update(number);
}

/*
Physics engine__
*/
PhysicsEngine::~PhysicsEngine()
{
	while (dEntities.size() > 0)
	{
		DynamicEntity *de = dEntities.back();
		dEntities.pop_back();
		delete de;
	}
	while (sEntities.size() > 0)
	{
		StaticEntity *se = sEntities.back();
		sEntities.pop_back();
		delete se;
	}
}

void PhysicsEngine::addDynamicEntity(DynamicEntity *de)
{
	dEntities.push_back(de);
}

void PhysicsEngine::addStaticEntity(StaticEntity *se)
{
	sEntities.push_back(se);
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
		DynamicEntity *de;
		StaticEntity *se;
		int deXpos, deYpos, deH, deW;
		int seXpos, seYpos, seH, seW;
		for (auto de : dEntities)
		{
			deXpos = de->getXPosition();
			deYpos = de->getYPosition();
			deH = de->getHeight();
			deW = de->getWidth();
			for (auto se : sEntities)
			{
				seXpos = se->getXPosition();
				seYpos = se->getYPosition();
				seH = se->getHeight();
				seW = se->getWidth();
				if (deYpos + deH < seYpos || deYpos > seYpos + seH)
				{
				}
				else if (deXpos + deW < seXpos || deXpos > seXpos + seW)
				{
				}
				else
				{
					collision(de, se);
				}

			}
		}
	}

}

void PhysicsEngine::collision(DynamicEntity *de, PhysicsEntity *pe)
{
	bool left = false, right = false, top = false, bottom = false;
	int deXpos = de->getXPosition(), deYpos = de->getYPosition(), deH = de->getHeight(), deW = de->getWidth();
	int peXpos = pe->getXPosition(), peYpos = pe->getYPosition(), peH = pe->getHeight(), peW = pe->getWidth();
	//Check corner collision


	if ((deYpos + deH) >= peYpos && deYpos < peYpos)
		bottom = true;
	else if (deYpos <= (peYpos + peH) && deYpos < peYpos)
		top = true;
	else if (deXpos <= peXpos + peW && deXpos > peXpos)
		left = true;
	else if (deXpos + deW >= peXpos && deXpos < peXpos)
		right = true;

	setResolver(rFactory.createResolver(top, bottom, left, right));

	if (resolver != nullptr)
		resolver->resolve(de, pe->getXVelocity(), pe->getYVelocity());
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
void TopLeftResolver::resolve(PhysicsEntity *pe, int xv, int yv)
{
	if (yv > 0 && pe->getYVelocity() > 0)
		pe->setYVelocity(yv + pe->getYVelocity());
	else if (yv < 0 && pe->getYVelocity() < 0)
		pe->setYVelocity(yv - pe->getYVelocity());
	else
		pe->setYVelocity(-1 * (pe->getYVelocity() - yv));
	if (xv < 0 && pe->getXVelocity() < 0)
		pe->setXVelocity(xv - pe->getXVelocity());
	else
		pe->setXVelocity(-1 * (pe->getXVelocity() - xv));

}

void TopRightResolver::resolve(PhysicsEntity *pe, int xv, int yv)
{
	if (yv > 0 && pe->getYVelocity() > 0)
		pe->setYVelocity(yv + pe->getYVelocity());
	else if (yv < 0 && pe->getYVelocity() < 0)
		pe->setYVelocity(yv - pe->getYVelocity());
	else
		pe->setYVelocity(-1 * (pe->getYVelocity() - yv));

	if (xv > 0 && pe->getXVelocity() > 0)
		pe->setXVelocity(xv - pe->getXVelocity());
	else if (xv < 0 && pe->getXVelocity() < 0)
		pe->setXVelocity(xv + pe->getXVelocity());
	else
		pe->setXVelocity(-1 * (pe->getXVelocity() - xv));
}

void BottomLeftResolver::resolve(PhysicsEntity *pe, int xv, int yv)
{

	if (yv < 0 && pe->getYVelocity() < 0)
		pe->setYVelocity(yv + pe->getYVelocity());
	else if (yv > 0 && pe->getYVelocity() > 0)
		pe->setYVelocity(yv - pe->getYVelocity());
	else
		pe->setYVelocity(-1 * (pe->getYVelocity() - yv));

	if (xv < 0 && pe->getXVelocity() < 0)
		pe->setXVelocity(xv - pe->getXVelocity());
	else
		pe->setXVelocity(-1 * (pe->getXVelocity() - xv));
}

void BottomRightResolver::resolve(PhysicsEntity *pe, int xv, int yv)
{
	if (yv < 0 && pe->getYVelocity() < 0)
		pe->setYVelocity(yv + pe->getYVelocity());
	else if (yv > 0 && pe->getYVelocity() > 0)
		pe->setYVelocity(yv - pe->getYVelocity());
	else
		pe->setYVelocity(-1 * (pe->getYVelocity() - yv));

	if (xv > 0 && pe->getXVelocity() > 0)
		pe->setXVelocity(xv - pe->getXVelocity());
	else if (xv < 0 && pe->getXVelocity() < 0)
		pe->setXVelocity(xv + pe->getXVelocity());
	else
		pe->setXVelocity(-1 * (pe->getXVelocity() - xv));
}

void LeftResolver::resolve(PhysicsEntity *pe, int xv, int yv)
{
	if (xv > 0 && pe->getXVelocity() > 0)
		pe->setXVelocity(xv - pe->getXVelocity());
	else if (xv < 0 && pe->getXVelocity() < 0)
		pe->setXVelocity(xv + pe->getXVelocity());
	else
		pe->setXVelocity(-1 * (pe->getXVelocity() - xv));
}

void RightResolver::resolve(PhysicsEntity *pe, int xv, int yv)
{
	if (xv > 0 && pe->getXVelocity() > 0)
		pe->setXVelocity(xv - pe->getXVelocity());
	else if (xv < 0 && pe->getXVelocity() < 0)
		pe->setXVelocity(xv + pe->getXVelocity());
	else
		pe->setXVelocity(-1 * (pe->getXVelocity() - xv));
}

void TopResolver::resolve(PhysicsEntity *pe, int xv, int yv)
{

	if (yv > 0 && pe->getYVelocity() > 0)
		pe->setYVelocity(yv + pe->getYVelocity());
	else if (yv < 0 && pe->getYVelocity() < 0)
		pe->setYVelocity(yv - pe->getYVelocity());
	else
		pe->setYVelocity(-1 * (pe->getYVelocity() - yv));

	//pe->setYVelocity(1);
}

void BottomResolver::resolve(PhysicsEntity *pe, int xv, int yv)
{
	if (pe->getYVelocity() > 0)
	{
		if (yv < 0 && pe->getYVelocity() < 0)
			pe->setYVelocity(yv + pe->getYVelocity());
		else if (yv > 0 && pe->getYVelocity() > 0)
			pe->setYVelocity(yv - pe->getYVelocity());
		else
			pe->setYVelocity(-1 * (pe->getYVelocity() - yv));
	}
	if (pe->getYVelocity() == 0)
		pe->setYAcceleration(0);

}