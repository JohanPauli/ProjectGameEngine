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
	renderer->render(_spriteSheet->getSprite(), &entity.physics->getRect(), _spriteSheet->getSpriteSrc(), entity.physics->getAngle());
}







// ---- PipeGraphics ----

PipeGraphics::PipeGraphics(Sprite* top, Sprite* mid, bool upward)
	: _top(top), _mid(mid), _upward(upward) {}

PipeGraphics::~PipeGraphics() {}

void PipeGraphics::update(Entity& entity) {
	// calculate position if not initialized or if entity's position has changed
	calcPos(entity.physics);

}

void PipeGraphics::render(Entity& entity, Renderer* renderer) {
	if (_upward)
		renderer->render(_top, &_topPos);
	else
		renderer->render(_top, &_topPos, nullptr, 0.0, nullptr, RenderFlip::VERTICAL);
	renderer->render(_mid, &_midPos);
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


//Background

BackgroundGraphics::BackgroundGraphics(Sprite* sky, Sprite* land) : _sky(sky), _land(land)
{}

BackgroundGraphics::~BackgroundGraphics() {}

void BackgroundGraphics::update(Entity &entity)
{
	int xPos = entity.physics->getXPosition(),
		yPos = entity.physics->getYPosition(),
		height = entity.physics->getHeight(),
		width = entity.physics->getWidth();

	int skyHeight = height * 0.75;
	int landHeight = height * 0.25;

	skyPos = Rect(xPos, yPos, width, skyHeight);
	landPos = Rect(xPos, yPos + skyHeight, width, landHeight);

}

void BackgroundGraphics::render(Entity &entity, Renderer *renderer)
{
	renderer->render(_sky, &skyPos);
	renderer->render(_land, &landPos);

}