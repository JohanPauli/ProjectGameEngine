#include "Entities.h"

#include "Rect.h"
#include "Rendering.h"
#include "Physics.h"
#include "Sprites.h"

#include <cmath>


// ---- Player ----

PlayerEntity::PlayerEntity(DynamicPhysics* physics, Sprite* sprite) {
	_physics = physics;


	std::vector<Rect> rector;

	// each animation is 18x12
	rector.emplace_back(Rect(0, 0, 18, 12));
	rector.emplace_back(Rect(18, 0, 18, 12));
	rector.emplace_back(Rect(36, 0, 18, 12));

	_spriteSheet = new SpriteSheet(rector, sprite);
}



PlayerEntity::~PlayerEntity() { 
	// tell PhysicsEngine to deallocate the physics object?
}



void PlayerEntity::update() {
	_updates++;
	if (_updates % 20 == 0)
		_spriteSheet->nextSprite();
}



void PlayerEntity::render(Renderer* renderer) {
	// maybe physics should return a Rect
	Rect pos = Rect((int)_physics->getXPosition(), (int)_physics->getYPosition(), (int)_physics->getWidth(), (int)_physics->getHeight());
	double angle = double(_updates % 360);

	renderer->render(_spriteSheet->getSprite(), &pos, _spriteSheet->getSpriteSrc(), angle);
}



void PlayerEntity::jump() {
	_physics->setYVelocity(-4);
}



// ---- PipeEntity ----



PipeEntity::PipeEntity(StaticPhysics* physics, Sprite* top, Sprite* mid, Sprite* bot) 
	: _physics(physics), _top(top), _mid(mid), _bot(bot) {}

PipeEntity::~PipeEntity() {
	// tell PhysicsEngine to deallocate the physics object?
}

void PipeEntity::update() {
	// do nothing
}


void PipeEntity::render(Renderer* renderer) {

	// entity divided by sprite
	float entityScaleFactor = _physics->getWidth() / _top->getWidth();
	float entityHeight = _physics->getHeight();
	int topHeight = (int)(_top->getHeight() * entityScaleFactor);
	int botHeight = (int)(_bot->getHeight() * entityScaleFactor);
	int midHeight = (int)(entityHeight - topHeight - botHeight);

	int topY = _physics->getYPosition();
	int midY = topY + topHeight;
	int botY = midY + midHeight;

	int x = _physics->getXPosition();
	int width = (int)_physics->getWidth();

	Rect topPos = Rect(x, topY, width, topHeight);
	Rect midPos = Rect(x, midY, width, midHeight);
	Rect botPos = Rect(x, botY, width, botHeight);

	renderer->render(_top, &botPos);
	renderer->render(_mid, &midPos);
	renderer->render(_bot, &topPos);
}