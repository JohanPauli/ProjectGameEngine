#pragma once

#include "InputContext.h"

// forward declarations
class Entity;


// base components

class Input : public InputContext {
protected:
	Entity* _entity = nullptr;

public:
	virtual ~Input() {}

	// set the entity 
	void setEntity(Entity* entity) { _entity = entity; }

	// InputContext: deal with user input
	virtual bool onNotify(const KeyboardInput&) override = 0;
};



// concrete classes


class PlayerInput : public Input {
private:
	void flap(bool repeat) const;

public:
	virtual ~PlayerInput();

	virtual bool onNotify(const KeyboardInput& input) override;
};