#include "GraphicsComponent.h"

#include <cmath>

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
	double angle = entity.getAngle();

	// flip the sprite depending on direction
	if (angle < -90 || angle > 90)
		renderer->render(_spriteSheet->getSprite(), &entity.getRect(), _spriteSheet->getSpriteSrc(), angle, nullptr, RenderFlip::VERTICAL);
	else
		renderer->render(_spriteSheet->getSprite(), &entity.getRect(), _spriteSheet->getSpriteSrc(), angle);
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

	pos = entity.getRect();
}

void BackgroundGraphics::render(Entity &entity, Renderer *renderer)
{
	renderer->render(background, &pos);

}



// variation of background

BackgroundGraphics2::BackgroundGraphics2(Sprite* background, float scale, int slowDown) 
: _background(background), _scale(scale), _slowDown(slowDown) {
	_bgWidth = (int)(_background->getWidth()*_scale);
	_bgHeight = (int)(_background->getHeight()*_scale);
}

BackgroundGraphics2::~BackgroundGraphics2() {}

void BackgroundGraphics2::update(Entity& entity) {

}

void BackgroundGraphics2::render(Entity& entity, Renderer* renderer) {
	int xOffset = renderer->getXoffset();
	int yOffset = renderer->getYoffset();

	int numberOfRenders = (int)(ceil(entity.getWidth() / _bgHeight));
	

	// move speed of background
	int bgOffset = (int)(xOffset / _slowDown) % (int)_bgWidth;

	for (int i = 0; i < numberOfRenders; i++) {
		int x = (int)(xOffset + (i - 1)*_bgWidth) - bgOffset;
		int y = entity.getHeight() - _bgHeight + yOffset;
		Rect pos = Rect(x, y, (int)_bgWidth, (int)_bgHeight);
		renderer->render(_background, &pos);
		// extend the top 1px of the background upwards to fill in the rest
		Rect src = Rect(0, 0, _bgWidth, 1);
		pos.setY(yOffset);
		pos.setHeight(entity.getHeight() - _bgHeight);
		renderer->render(_background, &pos, &src);
	}

}


// --- foreground ---

ForegroundGraphics::ForegroundGraphics(Sprite* background, float scale, int slowDown)
	: _background(background), _scale(scale), _slowDown(slowDown) {
	_bgWidth = (int)(_background->getWidth()*_scale);
	_bgHeight = (int)(_background->getHeight()*_scale);
}

ForegroundGraphics::~ForegroundGraphics() {}

void ForegroundGraphics::update(Entity& entity) {

}

void ForegroundGraphics::render(Entity& entity, Renderer* renderer) {
	int xOffset = renderer->getXoffset();
	int yOffset = renderer->getYoffset();

	int numberOfRenders = (int)(ceil(entity.getWidth() / _bgHeight));

	// move speed of background
	int bgOffset = (int)(xOffset / _slowDown) % (int)_bgWidth;

	for (int i = 0; i < numberOfRenders; i++) {
		int x = (int)(xOffset + (i - 1)*_bgWidth) - bgOffset;
		int y = entity.getHeight() + yOffset - _bgHeight;
		Rect pos = Rect(x, y, (int)_bgWidth, (int)_bgHeight);
		renderer->render(_background, &pos);
	}

}

//---- Counter Graphics ----

CounterGraphics::CounterGraphics(SpriteSheet *numbers,  int number) : _numbers(numbers)
{
	setNumber(number);
}

CounterGraphics::~CounterGraphics()
{
	delete _numbers;
}
void CounterGraphics::update(Entity& entity)
{
	//do nothing
}

void CounterGraphics::render(Entity& entity, Renderer* renderer)
{
	int width = (int)entity.getWidth();
	int height = (int)entity.getHeight();
	int xPos = (int)entity.getX();
	int numOfDigits = (int)digits.size();
	Rect *pos;
	for (int i = 0; i < (int)digits.size(); i++)
	{
		pos = new Rect(xPos, entity.getY(), width, height);
		_numbers->setSprite(digits[i]);
		renderer->render(_numbers->getSprite(), pos, _numbers->getSpriteSrc());
		xPos += width;
		delete pos;
	}

}

void CounterGraphics::setNumber(int number)
{
	digits.clear();
	int mod = 10;
	int numofDigits = 1;
	do{
		digits.emplace(digits.begin(), number % mod);
		number /= 10;

	} while (number > 0);

}