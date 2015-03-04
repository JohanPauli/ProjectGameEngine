#ifndef RESOLVER_H
#define RESOLVER_H



/*Resolver classes*/

class Physics;

class Resolver
{
public:
	virtual ~Resolver() {}
	virtual void resolve(Physics *physics, int _xVelocity, int _yVelocity) = 0;
};

class TopLeftResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, int _xVelocity, int _yVelocity);
};

class TopRightResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, int _xVelocity, int _yVelocity);
};

class BottomLeftResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, int _xVelocity, int _yVelocity);
};

class BottomRightResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, int _xVelocity, int _yVelocity);
};

class TopResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, int _xVelocity, int _yVelocity);
};

class BottomResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, int _xVelocity, int _yVelocity);
};

class LeftResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, int _xVelocity, int _yVelocity);
};

class RightResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, int _xVelocity, int _yVelocity);
};

//ResolverFactory, has the purpose of creating the correct resolver 
//for a specific collision
class ResolverFactory
{
public:
	Resolver* createResolver(bool top, bool bottom, bool left, bool right);

};
#endif