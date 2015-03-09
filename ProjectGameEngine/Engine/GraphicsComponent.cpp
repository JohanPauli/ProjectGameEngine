#include "GraphicsComponent.h"

#include "Entity.h"
#include "Rendering.h"
#include "Sprites.h"
#include "Physics.h"





// ---- BirdGraphics ----

BirdGraphics::BirdGraphics(SpriteSheet* spriteSheet, UpdateTimer timer)
: _spriteSheet(spriteSheet), _flapTimer(std::move(timer)) {}


BirdGraphics::~BirdGraphics() {
	delete _spriteSheet;
}

void BirdGraphics::update(Entity& entity) {
	// animation
	if (_flapTimer.isTimeToUpdate()) {
		_spriteSheet->nextSprite();
		_flapTimer.updated();
	}
}

void BirdGraphics::render(Entity& entity, Renderer* renderer) {
	// render
	renderer->render(_spriteSheet->getSprite(), &entity._physics->getRect(), _spriteSheet->getSpriteSrc(), entity._physics->getAngle());
}







// ---- PipeGraphics ----

PipeGraphics::PipeGraphics(Sprite* top, Sprite* mid, bool upward)
	: _top(top), _mid(mid), _upward(upward) {}

PipeGraphics::~PipeGraphics() {}

void PipeGraphics::update(Entity& entity) {
	// calculate position if not initialized or if entity's position has changed
	if (hasMoved(entity._physics))
		calcPos(entity._physics);

}

void PipeGraphics::render(Entity& entity, Renderer* renderer) {
	if (_upward)
		renderer->render(_top, &_topPos);
	else
		renderer->render(_top, &_topPos, nullptr, 0.0, nullptr, RenderFlip::VERTICAL);
	renderer->render(_mid, &_midPos);
}



// check if the entity's physics has moved compared to Graphics's positions
bool PipeGraphics::hasMoved(const Physics* physics) const {
	// moved on the X-axis
	if (physics->getXPosition() != _topPos.getX())
		return true;
	// moved on the Y-axis, take into account _topPos and _midPos could be flipped
	if (physics->getYPosition() != _topPos.getY() &&
		physics->getYPosition() != _midPos.getY())
		return true;
	return false;
}


// calculate the positions of both sprites
void PipeGraphics::calcPos(const Physics* physics) {
	// heights
	int topHeight = (int)(_top->getHeight() * (physics->getWidth() / _top->getWidth()));
	int midHeight = (int)(physics->getHeight() - topHeight);

	// y positions
	int topY, midY;
	if (_upward) {
		topY = (int)physics->getYPosition();
		midY = topY + topHeight;
	}
	else {
		midY = (int)physics->getYPosition();
		topY = midY + midHeight;
	}

	int x =		(int)physics->getXPosition();
	int width =	(int)physics->getWidth();

	// set positions
	_topPos = Rect(x, topY, width, topHeight);
	_midPos = Rect(x, midY, width, midHeight);
}
