#ifndef __ENTITY_H__
#define __ENTITY_H__


#include "Rendering.h"
#include "Physics.h"

#include "Debug.h"

typedef PhysicsEntity	Physics;
typedef StaticEntity	StaticPhysics;
typedef DynamicEntity	DynamicPhysics;

class Entity {
public:
	virtual ~Entity() {}

	virtual void update() = 0;	// update logic
	virtual void render(Renderer*) = 0;		// render the entity
};



class Player : public Entity {
private:
	DynamicPhysics* _physics = nullptr;
	Texture*		_texture = nullptr;
	Rect			_animation[3];
	int				_aniIdx = 0;
	int				_updates = 0;


public:
	Player(DynamicPhysics* physics, Texture* texture) {
		_texture = texture;
		_physics = physics;
		_animation[0] = Rect(0, 0, 18, 12);	// each animation is 18x12
		_animation[1] = Rect(18, 0, 18, 12);
		_animation[2] = Rect(36, 0, 18, 12); 
	}

	virtual void update() override {
		_updates++;
		if (_updates % 20 == 0)
			_aniIdx = (_aniIdx + 1) % 3;
	}

	virtual void render(Renderer* renderer) override {
		// maybe physics should return a Rect
		Rect pos = Rect(_physics->getXPosition(), _physics->getYPosition(), _physics->getWidth(), _physics->getHeight());
		double angle = double(_updates % 360);

		renderer->render(_texture, &pos, &_animation[_aniIdx], angle);
	}

	virtual void jump() {
		_physics->setYVelocity(-4);
	}
};



#endif