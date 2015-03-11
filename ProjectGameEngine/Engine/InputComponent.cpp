#include "InputComponent.h"

#include "InputFetcher.h"
#include "Entity.h"


// ---- PlayerInput ----

PlayerInput::~PlayerInput() {}

void PlayerInput::update(Entity& entity) {
	// do nothing
}

bool PlayerInput::onNotify(const KeyboardInput& input) {
	switch (input.key) {
	case KeyboardKey::SPACE:
		flap(input.repeat);
		return true;
	case KeyboardKey::UP:
		flap(input.repeat);
		return true;
	case KeyboardKey::W:
		flap(input.repeat);
		return true;
	case KeyboardKey::A:
		left(input.repeat);
		return true;
	case KeyboardKey::S:
		stop(input.repeat);
		return true;
	case KeyboardKey::D:
		right(input.repeat);
		return true;
	default: return false;;
	}
}

void PlayerInput::flap(bool repeat) const {
	float yVel = _entity->getYvelocity();
	if (!repeat)
		_entity->setYvelocity(-5);
}

void PlayerInput::right(bool repeat) const {
	float xVel = _entity->getXvelocity();
	if (xVel < 5)
		xVel += (0.03);
	_entity->setXvelocity(xVel);
}

void PlayerInput::left(bool repeat) const {
	float xVel = _entity->getXvelocity();
	if (xVel > -5)
		xVel -= (0.03);
	_entity->setXvelocity(xVel);
}

void PlayerInput::stop(bool repeat) const {
	if (!repeat) {
		_entity->setXvelocity(0);
		_entity->setYvelocity(0);
	}
}