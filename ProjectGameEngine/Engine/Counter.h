#ifndef COUNTER_H
#define COUNTER_H

#include "Entity.h"
#include "Physics.h"
#include "GraphicsComponent.h"

class Counter : public EntityInterface
{
private:
	Entity* _entity;
	CounterGraphics *cGraphics;
	void init();
	int oldScore = 0;
public:
	Counter(Entity*);
	~Counter();
	virtual void update();
	virtual void render(Renderer* renderer);
	void setNumber(int);
	int getNumber() const;
	

};
#endif