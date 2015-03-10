#include "Entity.h"

#include "Rendering.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "Physics.h"


Entity::Entity(Physics* physics, Graphics* graphics, Input* input)
: _physics(physics), _graphics(graphics), _input(input) {
	// init input component
	if (_input != nullptr)
		_input->setEntity(this);
}


Entity::~Entity() {
	delete _physics;
	delete _graphics;
	delete _input;
}


void Entity::update(int xMove, int yMove) {
	// input
	if (_input != nullptr)
		_input->update(*this);

	// physics
	_physics->update(0);

	// graphics
	if (_graphics != nullptr)
		_graphics->update(*this);
}



void Entity::render(Renderer* renderer) {
	_graphics->render(*this, renderer);
}