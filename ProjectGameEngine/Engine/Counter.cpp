#include "Counter.h"
#include "Sprites.h"
#include "RessourceManager.h"
#include "EntityGenerators.h"

Counter::Counter(int x, int y, double scale, int n) : x(x), y(y), scale(scale)
{
	std::vector<Rect> digitsPos;
	Sprite *numbers = RessourceManager::getInstance().getByTag<Sprite*>("numbers");
	float width = ((float)numbers->getWidth() / 10);
	float xPos = 1;
	int height = 18;
	for (int i = 1; i < 11; i++)
	{
		digitsPos.emplace_back(Rect(xPos, 1, width - 1, height));
		xPos = width*i + 1;
	}
	_entity = EntityGenerator::getInstance().createCounterEntity(n, x, y, scale);
}

Counter::~Counter()
{
	delete _entity;
	
}

void Counter::render(Renderer* renderer)
{
	_entity->render(renderer );
}

void Counter::update()
{
	//nothing to update
}

void Counter::setNumber(int number)
{
	if (number != oldScore)
	{
		_entity = EntityGenerator::getInstance().createCounterEntity(number, x, y, scale);
		oldScore = number;
		Audio& audio = Audio::get();
		audio.stopSound(2);
		audio.playSound(RessourceManager::getInstance().getByTag<Sound*>("point"), 0, 2);
	}
}

int Counter::getNumber() const {
	return oldScore;
}
