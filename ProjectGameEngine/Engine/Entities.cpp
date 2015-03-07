#include "Entities.h"

#include "Rendering.h"
#include "Physics.h"
#include "Sprites.h"
#include "InputFetcher.h"

#include <cmath>


// ---- Player ----

PlayerEntity::PlayerEntity(DynamicPhysics* physics, SpriteSheet* spriteSheet) 
: _physics(physics), _spriteSheet(spriteSheet) { }



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

	renderer->render(_spriteSheet->getSprite(), &pos, _spriteSheet->getSpriteSrc(), _physics->getAngle());
}



bool PlayerEntity::onNotify(const KeyboardInput& key) {

	switch (key.key) {
	case KeyboardKey::SPACE:
		flap(key.repeat);
		return true;
	case KeyboardKey::UP:
		flap(key.repeat);
		return true;
	case KeyboardKey::W:
		flap(key.repeat);
		return true;
	default: return false;;
	}
}

void PlayerEntity::flap(bool repeat) {
	if (!repeat)
		_physics->setYVelocity(-5);
}


// ---- PipeEntity ----


PipeEntity::PipeEntity(StaticPhysics* physics, Sprite* top, Sprite* mid, bool upward)
: _physics(physics), _top(top), _mid(mid), _upward(upward) {
	/*
		TODO:
		these calculations could probably be made more generally applicable
		we shouldnt have to write the same code in every entity
	*/

	// entity divided by sprite
	float entityScaleFactor = _physics->getWidth() / _top->getWidth();

	// heights
	int topHeight = (int)(_top->getHeight() * entityScaleFactor);
	int midHeight = (int)(_physics->getHeight() - topHeight);

	// y positions
	int topY, midY;
	if (_upward) {
		topY = (int)_physics->getYPosition();
		midY = topY + topHeight;
	}
	else {
		midY = (int)_physics->getYPosition();
		topY = midY + midHeight;
	}
	
	int x = (int)_physics->getXPosition();
	int width = (int)_physics->getWidth();


	_topPos = Rect(x, topY, width, topHeight);;
	_midPos = Rect(x, midY, width, midHeight);;
}

PipeEntity::~PipeEntity() {
	// tell PhysicsEngine to deallocate the physics object?
}


void PipeEntity::update() {
	// do nothing
}

void PipeEntity::render(Renderer* renderer) {
	if (_upward)
		renderer->render(_top, &_topPos);
	else
		renderer->render(_top, &_topPos, nullptr, 0.0, nullptr, RenderFlip::VERTICAL);
	renderer->render(_mid, &_midPos);
}



// ---- DEPipeEntity ----

DEPipeEntity::DEPipeEntity(StaticPhysics* physics, Sprite* top, Sprite* mid, Sprite* bot) 
	: _physics(physics), _top(top), _mid(mid), _bot(bot) 
{
	// entity divided by sprite
	float entityScaleFactor = _physics->getWidth() / _top->getWidth();

	// heights
	int topHeight = (int)(_top->getHeight() * entityScaleFactor);
	int botHeight = (int)(_bot->getHeight() * entityScaleFactor);
	int midHeight = (int)(_physics->getHeight() - topHeight - botHeight);

	// y positions
	int topY = (int)_physics->getYPosition();
	int midY = topY + topHeight;
	int botY = midY + midHeight;

	int x = (int)_physics->getXPosition();
	int width = (int)_physics->getWidth();

	_topPos = Rect(x, topY, width, topHeight);
	_midPos = Rect(x, midY, width, midHeight);
	_botPos = Rect(x, botY, width, botHeight);
}

DEPipeEntity::~DEPipeEntity() {
	// tell PhysicsEngine to deallocate the physics object?
}

void DEPipeEntity::update() {
	// do nothing
}


void DEPipeEntity::render(Renderer* renderer) {
	renderer->render(_top, &_topPos);
	renderer->render(_mid, &_midPos);
	renderer->render(_bot, &_botPos);
}