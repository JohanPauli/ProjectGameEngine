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

Rect Entity::getRect() const	{ return std::move(physics->getRect()); }
double Entity::getAngle() const { return physics->getAngle(); }
int Entity::getX() const		{ return (int)physics->getXPosition(); }
int Entity::getY() const		{ return (int)physics->getYPosition(); }
int Entity::getWidth() const	{ return (int)physics->getWidth(); }
int Entity::getHeight() const	{ return (int)physics->getHeight(); }
float Entity::getXvelocity() const { return physics->getXVelocity(); }
float Entity::getYvelocity() const { return physics->getYVelocity(); }
void Entity::setXvelocity(float vel) const { physics->setXVelocity(vel); }
void Entity::setYvelocity(float vel) const { physics->setYVelocity(vel); }
InputContext* Entity::getInputContext() const { return input; }