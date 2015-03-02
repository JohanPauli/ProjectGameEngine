#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

using std::vector;

enum SIDES { TOP, BOTTOM, LEFT, RIGHT };

class PhysicsEntity
{
protected:
	int xAcceleration;
	int yAcceleration;

	int xVelocity;
	int yVelocity;

	int xPosition;
	int yPosition;

	int height;
	int width;
public:
	//Constructor
	PhysicsEntity(int xa, int ya, int xv, int yv, int xp, int yp, int h, int w) :
		xAcceleration(xa), yAcceleration(ya), xVelocity(xv), yVelocity(yv),
		xPosition(xp), yPosition(yp), height(h), width(w) {}
	virtual ~PhysicsEntity() {}

	//getters
	int getXAcceleration() const { return xAcceleration; }
	int getYAcceleration() const { return yAcceleration; }
	int getXVelocity() const { return xVelocity; }
	int getYVelocity() const { return yVelocity; }
	int getXPosition() const { return xPosition; }
	int getYPosition() const { return yPosition; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }

	//setters
	void setXAcceleration(int xa) { xAcceleration = xa; }
	void setYAcceleration(int ya) { yAcceleration = ya; }
	void setXVelocity(int xv)  { xVelocity = xv; }
	void setYVelocity(int yv) { yVelocity = yv; }
	void setXPosition(int xp) { xPosition = xp; }
	void setYPosition(int yp) { yPosition = yp; }

	//Pure virtual update function
	virtual void update(int elapsed) = 0;


};

class DynamicEntity : public PhysicsEntity
{
public:
	DynamicEntity(int xa, int ya, int xv, int yv, int xp, int yp, int h, int w) : PhysicsEntity(xa, ya, xv, yv, xp, yp, h, w){}
	void jump();
	virtual void update(int elapsed);
	void resolveCollision(int collVelocity, SIDES);

};

class StaticEntity : public PhysicsEntity
{
public:
	StaticEntity(int xa, int ya, int xv, int yv, int xp, int yp, int h, int w) : PhysicsEntity(xa, ya, xv, yv, xp, yp, h, w){}
	virtual void update(int elapsed);
};

class Resolver;

class ResolverFactory
{
public:
	Resolver* createResolver(bool top, bool bottom, bool left, bool right);

};

class PhysicsEngine
{
public:
	~PhysicsEngine();
	void update();
	void detectColissions();
	void collision(DynamicEntity *, PhysicsEntity *);
	void addDynamicEntity(DynamicEntity *de);
	void addStaticEntity(StaticEntity *se);
	void setResolver(Resolver *res);

private:
	vector<DynamicEntity*> dEntities;
	vector<StaticEntity*> sEntities;
	ResolverFactory rFactory;
	Resolver *resolver;

};


/*Resolver classes*/

class Resolver
{
public:
	virtual ~Resolver() {}
	virtual void resolve(PhysicsEntity *pe, int xv, int yv) = 0;
};

class TopLeftResolver : public Resolver
{
public:
	virtual void resolve(PhysicsEntity *pe, int xv, int yv);
};

class TopRightResolver : public Resolver
{
public:
	virtual void resolve(PhysicsEntity *pe, int xv, int yv);
};

class BottomLeftResolver : public Resolver
{
public:
	virtual void resolve(PhysicsEntity *pe, int xv, int yv);
};

class BottomRightResolver : public Resolver
{
public:
	virtual void resolve(PhysicsEntity *pe, int xv, int yv);
};

class TopResolver : public Resolver
{
public:
	virtual void resolve(PhysicsEntity *pe, int xv, int yv);
};

class BottomResolver : public Resolver
{
public:
	virtual void resolve(PhysicsEntity *pe, int xv, int yv);
};

class LeftResolver : public Resolver
{
public:
	virtual void resolve(PhysicsEntity *pe, int xv, int yv);
};

class RightResolver : public Resolver
{
public:
	virtual void resolve(PhysicsEntity *pe, int xv, int yv);
};


#endif