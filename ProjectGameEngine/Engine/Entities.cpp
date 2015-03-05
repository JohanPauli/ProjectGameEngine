#include "Entities.h"

#include "Rendering.h"
#include "Physics.h"
#include "Sprites.h"

#include <cmath>


// ---- Player ----

PlayerEntity::PlayerEntity(DynamicPhysics* physics, Sprite* sprite) 
	: InputContext(InputContextType::BIRD), _physics(physics)
{
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

bool PlayerEntity::onNotify(KeyboardInput key) {

	switch (key) {
	case KeyboardInput::SPACE:
		flap();
		return true;
	case KeyboardInput::UP:
		flap();
		return true;
	case KeyboardInput::W:
		flap();
		return true;
	default: return false;;
	}
}

void PlayerEntity::flap() {
	_physics->setYVelocity(-5);
}


// ---- PipeEntity ----



PipeEntity::PipeEntity(StaticPhysics* physics, Sprite* top, Sprite* mid, Sprite* bot) 
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

PipeEntity::~PipeEntity() {
	// tell PhysicsEngine to deallocate the physics object?
}

void PipeEntity::update() {
	// do nothing
}


void PipeEntity::render(Renderer* renderer) {
	renderer->render(_top, &_topPos);
	renderer->render(_mid, &_midPos);
	renderer->render(_bot, &_botPos);
}