#include "Entity.h"

#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "Physics.h"


Entity::Entity(Graphics* graphics, Physics* physics, Input* input)
: _graphics(graphics), _physics(physics), _input(input) {
	// init input component
	if (_input != nullptr)
		_input->setEntity(this);
}


Entity::~Entity() {}


void Entity::update() {
	// questionmark
	// update states or something
}



void Entity::render(Renderer* renderer) {
	_graphics->render(*this, renderer);
}