#pragma once


#include <SDL.h>


/*
Black box for SDL_Point
don't know where to put this class
*/
class Point {
	friend class Renderer;
private:
	SDL_Point point;
public:
	Point(int x, int y);

	int getX() const	{ return point.x; }
	int getY() const	{ return point.y; }

	void setX(int x)	{ point.x = x; }
	void setY(int y)	{ point.y = y; }

	void adjustX(int value)	{ point.x += value; }
	void adjustY(int value)	{ point.y += value; }
};



/*
	Blackbox for SDL_Rect
*/
class Rect {
	friend class Renderer;
private:
	SDL_Rect rect;

public:
	Rect() 
		: rect({rect.x = 0, rect.y = 0, rect.w = 0, rect.h = 0}) {}
	Rect(int x, int y, int width, int height) 
		: rect({ rect.x = x, rect.y = y, rect.w = width, rect.h = height }) {}

	// center of the rectangle
	Point getCenter() const {
		int x, y;
		x = rect.x + (rect.w / 2);
		y = rect.y + (rect.h / 2);
		return Point(x, y);
	}

	// accessors

	int getX() const		{ return rect.x; }
	int getY() const		{ return rect.y; }
	int getWidth() const	{ return rect.w; }
	int getHeight() const	{ return rect.h; }

	void setX(int value)		{ rect.x = value; }
	void setY(int value)		{ rect.y = value; }
	void setWidth(int value)	{ rect.w = value; }
	void setHeight(int value)	{ rect.h = value; }

	void adjustX(int value)		{ rect.x += value; }
	void adjustY(int value)		{ rect.y += value; }
	void adjustWidth(int value)	{ rect.w += value; }
	void adjustHeight(int value){ rect.h += value; }
};