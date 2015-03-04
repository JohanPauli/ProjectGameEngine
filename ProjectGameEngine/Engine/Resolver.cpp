#include "Resolver.h"
#include "Physics.h"


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
void TopLeftResolver::resolve(Physics *pe, int xv, int yv)
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

void TopRightResolver::resolve(Physics *pe, int xv, int yv)
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

void BottomLeftResolver::resolve(Physics *pe, int xv, int yv)
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

void BottomRightResolver::resolve(Physics *pe, int xv, int yv)
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

void LeftResolver::resolve(Physics *pe, int xv, int yv)
{
	if (xv > 0 && pe->getXVelocity() > 0)
		pe->setXVelocity(xv - pe->getXVelocity());
	else if (xv < 0 && pe->getXVelocity() < 0)
		pe->setXVelocity(xv + pe->getXVelocity());
	else
		pe->setXVelocity(-1 * (pe->getXVelocity() - xv));
}

void RightResolver::resolve(Physics *pe, int xv, int yv)
{
	if (xv > 0 && pe->getXVelocity() > 0)
		pe->setXVelocity(xv - pe->getXVelocity());
	else if (xv < 0 && pe->getXVelocity() < 0)
		pe->setXVelocity(xv + pe->getXVelocity());
	else
		pe->setXVelocity(-1 * (pe->getXVelocity() - xv));
}

void TopResolver::resolve(Physics *pe, int xv, int yv)
{

	if (yv > 0 && pe->getYVelocity() > 0)
		pe->setYVelocity(yv + pe->getYVelocity());
	else if (yv < 0 && pe->getYVelocity() < 0)
		pe->setYVelocity(yv - pe->getYVelocity());
	else
		pe->setYVelocity(-1 * (pe->getYVelocity() - yv));

	//pe->setYVelocity(1);
}

void BottomResolver::resolve(Physics *pe, int xv, int yv)
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