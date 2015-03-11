#include "InputComponent.h"

#include "InputFetcher.h"
#include "Entity.h"
#include "Physics.h"	// manipulate velocities


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
	default: return false;;
	}
}

void PlayerInput::flap(bool repeat) const {
	if (repeat)
		_entity->setYvelocity(-5);
}