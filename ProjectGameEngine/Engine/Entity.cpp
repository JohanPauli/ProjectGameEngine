#include "Entity.h"

#include "Rendering.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "Physics.h"


Entity::Entity(Physics* physics, Graphics* graphics, Input* input)
: physics(physics), graphics(graphics), input(input) {
	// init input component
	if (input != nullptr)
		input->setEntity(this);
}


Entity::~Entity() {
	delete physics;
	delete graphics;
	delete input;
}


void Entity::update() {
	// input
	if (input != nullptr)
		input->update(*this);

	// physics
	physics->update(0);

	// graphics
	if (graphics != nullptr)
		graphics->update(*this);
}



void Entity::render(Renderer* renderer) {
	graphics->render(*this, renderer);
}

Rect Entity::getRect() const	{ return physics == nullptr ? Rect() :	std::move(physics->getRect()); }
double Entity::getAngle() const { return physics == nullptr ? 0 :	physics->getAngle(); }
int Entity::getX() const		{ return physics == nullptr ? 0 :(int)physics->getXPosition(); }
int Entity::getY() const		{ return physics == nullptr ? 0 :(int)physics->getYPosition(); }
int Entity::getWidth() const	{ return physics == nullptr ? 0 :(int)physics->getWidth(); }
int Entity::getHeight() const	{ return physics == nullptr ? 0 :(int)physics->getHeight(); }
float Entity::getXvelocity() const { return physics == nullptr ? 0 : physics->getXVelocity(); }
float Entity::getYvelocity() const { return physics == nullptr ? 0 : physics->getYVelocity(); }
void Entity::setXvelocity(float vel) const { if (physics != nullptr) physics->setXVelocity(vel); }
void Entity::setYvelocity(float vel) const { if (physics != nullptr) physics->setYVelocity(vel); }
InputContext* Entity::getInputContext() const { return input; }