#ifndef COUNTER_H
#define COUNTER_H

#include "Entity.h"
#include "Physics.h"
#include "GraphicsComponent.h"

class Counter
{
private:
	Entity* _entity;
	int oldScore = 0;
	int x;
	int y;
	double scale;
public:
	Counter(int x, int y, double scale, int n = 0);
	~Counter();
	void update();
	void render(Renderer* renderer);
	void setNumber(int);
	int getNumber() const;
	

};
#endif