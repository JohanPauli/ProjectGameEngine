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
	renderer->render(_spriteSheet->getSprite(), &entity.getRect(), _spriteSheet->getSpriteSrc(), entity.getAngle());
}







// ---- PipeGraphics ----

PipeGraphics::PipeGraphics(Sprite* top, Sprite* mid, bool upward)
	: _top(top), _mid(mid), _upward(upward) {}

PipeGraphics::~PipeGraphics() {}

void PipeGraphics::update(Entity& entity) {
	// calculate position if not initialized or if entity's position has changed
	calcPos(entity);

}

void PipeGraphics::render(Entity& entity, Renderer* renderer) {
	if (_upward)
		renderer->render(_top, &_topPos);
	else
		renderer->render(_top, &_topPos, nullptr, 0.0, nullptr, RenderFlip::VERTICAL);
	renderer->render(_mid, &_midPos);
}





// calculate the positions of both sprites
void PipeGraphics::calcPos(const Entity& entity) {
	// heights
	int topHeight = (int)(_top->getHeight() * (entity.getWidth() / _top->getWidth()));
	int midHeight = entity.getHeight() - topHeight;

	// y positions
	int topY, midY;
	if (_upward) {
		topY = entity.getY();
		midY = topY + topHeight;
	}
	else {
		midY = entity.getY();
		topY = midY + midHeight;
	}

	int x =		entity.getX();
	int width =	entity.getWidth();

	// set positions
	_topPos = Rect(x, topY, width, topHeight);
	_midPos = Rect(x, midY, width, midHeight);
}


//Background

BackgroundGraphics::BackgroundGraphics(Sprite* bg) : background(bg)
{
}

BackgroundGraphics::~BackgroundGraphics() {}

void BackgroundGraphics::update(Entity &entity)
{

	pos = entity.physics->getRect();
}

void BackgroundGraphics::render(Entity &entity, Renderer *renderer)
{
	renderer->render(background, &pos);

}