#ifndef PHYSICS_H
#define PHYSICS_H

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
	void setYAcceleration(int ya) { yAcceleration= ya; }
	void setXVelocity(int xv)  { xVelocity = xv; }
	void setYVelocity(int yv) { yVelocity = yv; }
	void setXPosition(int xp) { xPosition = xp; }
	void setYPosition(int yp) { yPosition = yp; }

	//Pure virtual update function
	virtual void update(int elapsed) = 0;


};
#endif