#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__


#include <vector>

#include "Rendering.h"
#include "Debug.h"


class SpriteSheet {
private:
	std::vector<Rect>	_animations;
	int					_currentAnimation = 0;
	Sprite*				_sprite = nullptr;

public:
	SpriteSheet(std::vector<Rect> animations,  Sprite* sprite) 
		: _animations(std::move(animations)), _sprite(sprite) {
		if (_animations.size() < 1) LOG("Animations vector contains no entry");
	}

	// set sprite to next sprite
	void nextSprite() {
		_currentAnimation = ++_currentAnimation % (int)_animations.size();
	}
	
	// set sprite to previous sprite
	void prevSprite() {
		--_currentAnimation;
		if (_currentAnimation < 0)
			_currentAnimation = (int)_animations.size() - 1;
	}

	// set the sprite index
	void setSprite(int idx) {
		if (idx < (int)_animations.size())
			_currentAnimation = idx;
	}

	// return the spritesheet
	Sprite*	getSprite() const {
		return _sprite;
	}

	// return the Rect showing the position of the sprite on the spritesheet
	Rect*	getSpriteSrc() {
		return &_animations[_currentAnimation];
	}

};


class Background {
private:

public:



};



#endif