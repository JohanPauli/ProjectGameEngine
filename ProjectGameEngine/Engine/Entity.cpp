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


void Entity::update(int xMove, int yMove) {
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