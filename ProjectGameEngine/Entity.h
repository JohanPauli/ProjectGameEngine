#ifndef __ENTITY_H__
#define __ENTITY_H__


#include "Rendering.h"
#include "Physics.h"
#include "Graphics.h"

#include "Debug.h"


// entity interface
class Entity {
public:
	virtual void update() = 0;			// update logic
	virtual void render(Renderer*) = 0;	// render the entity
};



// derived entities below

class Player : public Entity {
private:
	DynamicPhysics* _physics = nullptr;
	SpriteSheet*	_spriteSheet = nullptr;
	int				_updates = 0;


public:
	Player(DynamicPhysics* physics, Sprite* sprite) {
		_physics = physics;


		std::vector<Rect> rector;
		
		// each animation is 18x12
		rector.emplace_back(Rect(0, 0, 18, 12));	
		rector.emplace_back(Rect(18, 0, 18, 12));
		rector.emplace_back(Rect(36, 0, 18, 12));

		_spriteSheet = new SpriteSheet(rector, sprite);
	}
	~Player() {
		delete _spriteSheet;
	}

	virtual void update() override {
		_updates++;
		if (_updates % 20 == 0)
			_spriteSheet->nextSprite();
	}

	virtual void render(Renderer* renderer) override {
		// maybe physics should return a Rect
		Rect pos = Rect(_physics->getXPosition(), _physics->getYPosition(), _physics->getWidth(), _physics->getHeight());
		double angle = double(_updates % 360);

		renderer->render(_spriteSheet->getSprite(), &pos, _spriteSheet->getSpriteSrc(), angle);
	}

	virtual void jump() {
		_physics->setYVelocity(-4);
	}
};



#endif