#pragma once

#include "Rendering.h"


// entity interface
class Entity {
public:
	virtual void update() = 0;			// update logic
	virtual void render(Renderer*) = 0;	// render the entity
};