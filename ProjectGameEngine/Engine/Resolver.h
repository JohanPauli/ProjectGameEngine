#pragma once



/*Resolver classes*/

class Physics;

class Resolver
{
public:
	virtual ~Resolver() {}
	virtual void resolve(Physics *physics, Physics *otherPhysics) = 0;
};

class TopLeftResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, Physics *otherPhysics);
};

class TopRightResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, Physics *otherPhysics);
};

class BottomLeftResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, Physics *otherPhysics);
};

class BottomRightResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, Physics *otherPhysics);
};

class TopResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, Physics *otherPhysics);
};

class BottomResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, Physics *otherPhysics);
};

class LeftResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, Physics *otherPhysics);
};

class RightResolver : public Resolver
{
public:
	virtual void resolve(Physics *physics, Physics *otherPhysics);
};

//ResolverFactory, has the purpose of creating the correct resolver 
//for a specific collision
class ResolverFactory
{
public:
	Resolver* createResolver(bool top, bool bottom, bool left, bool right);

};