#include "Counter.h"
#include "Sprites.h"
#include "RessourceManager.h"

Counter::Counter(Entity* entity) : _entity(entity)
{
	init();
}

Counter::~Counter()
{
	delete _entity,
		cGraphics;
	
}

void Counter::init()
{
	std::vector<Rect> digitsPos;
	Sprite *numbers = RessourceManager::getInstance().getByTag<Sprite*>("numbers");
	float width = ((float) numbers->getWidth() / 10);
	float xPos = 1;
	int height = numbers->getHeight()-10;
	for (int i = 1; i < 11; i++)
	{
		digitsPos.emplace_back(Rect(xPos, 1, width - 1, height));
		xPos = width*i + 1;
	}
	cGraphics = new CounterGraphics(new SpriteSheet(digitsPos, numbers));
	

}

void Counter::render(Renderer* renderer)
{
	cGraphics->render(*_entity, renderer );
}

void Counter::update()
{
	//nothing to update
}

void Counter::setNumber(int number)
{
	if (number != oldScore)
	{
		cGraphics->setNumber(number);
		oldScore = number;
		Audio& audio = Audio::get();
		audio.stopSound(2);
		audio.playSound(RessourceManager::getInstance().getByTag<Sound*>("point"), 0, 2);
	}
}