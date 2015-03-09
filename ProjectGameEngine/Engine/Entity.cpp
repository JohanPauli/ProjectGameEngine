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


void Entity::update() {
	_input->update(*this);
	_physics->update(0);
	_graphics->update(*this);
}



void Entity::render(Renderer* renderer) {
	_graphics->render(*this, renderer);
}