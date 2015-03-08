#include "GraphicsComponent.h"

#include "Entity.h"
#include "Rendering.h"
#include "Sprites.h"
#include "Physics.h"





// ---- BirdGraphics ----

BirdGraphics::BirdGraphics(SpriteSheet* spriteSheet, Timer* timer)
: _spriteSheet(spriteSheet), _flapTimer(UpdateTimer(timer, MS_PER_FRAME)) {}


BirdGraphics::~BirdGraphics() {}


void BirdGraphics::render(Entity& entity, Renderer* renderer) {
	// animation
	if (_flapTimer.isTimeToUpdate()) {
		_spriteSheet->nextSprite();
		_flapTimer.updated();
	}
	// render
	renderer->render(_spriteSheet->getSprite(), &entity._physics->getRect(), _spriteSheet->getSpriteSrc(), entity._physics->getAngle());
}







// ---- PipeGraphics ----

PipeGraphics::PipeGraphics(Sprite* top, Sprite* mid, bool upward)
	: _top(top), _mid(mid), _upward(upward) {}

PipeGraphics::~PipeGraphics() {}


void PipeGraphics::render(Entity& entity, Renderer* renderer) {
	if (!_isInit)
		init(entity);
	if (_upward)
		renderer->render(_top, &_topPos);
	else
		renderer->render(_top, &_topPos, nullptr, 0.0, nullptr, RenderFlip::VERTICAL);
	renderer->render(_mid, &_midPos);
}


// calculate the positions of both sprites
void PipeGraphics::init(Entity& entity) {
	// calculate position
	float entityScaleFactor = entity._physics->getWidth() / _top->getWidth();

	// heights
	int topHeight = (int)(_top->getHeight() * entityScaleFactor);
	int midHeight = (int)(entity._physics->getHeight() - topHeight);

	// y positions
	int topY, midY;
	if (_upward) {
		topY = (int)entity._physics->getYPosition();
		midY = topY + topHeight;
	}
	else {
		midY = (int)entity._physics->getYPosition();
		topY = midY + midHeight;
	}

	int x = (int)entity._physics->getXPosition();
	int width = (int)entity._physics->getWidth();

	// set positions
	_topPos = Rect(x, topY, width, topHeight);
	_midPos = Rect(x, midY, width, midHeight);
	_isInit = true;
}
