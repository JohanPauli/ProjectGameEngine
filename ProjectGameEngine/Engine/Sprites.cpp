#include "Sprites.h"

#include "Rendering.h"
#include "Rect.h"
#include "Debug.h"


// ---- Sprite ----

Sprite::Sprite(SDL_Texture* texture, int width, int height)
: _texture(texture), _width(width), _height(height) {}


Sprite::~Sprite() {
	SDL_DestroyTexture(_texture);
}


// color modulation
void Sprite::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(_texture, red, green, blue);
}


// set blending
void Sprite::setBlendMode(BlendMode blending) {
	SDL_BlendMode sdlBlend;
	switch (blending) {
	case BlendMode::NONE:	sdlBlend = SDL_BLENDMODE_NONE; break;
	case BlendMode::ADD:	sdlBlend = SDL_BLENDMODE_ADD; break;
	case BlendMode::BLEND:	sdlBlend = SDL_BLENDMODE_BLEND; break;
	case BlendMode::MOD:	sdlBlend = SDL_BLENDMODE_MOD; break;
	}
	SDL_SetTextureBlendMode(_texture, sdlBlend);
}

// set alpha (opacity)
void Sprite::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(_texture, alpha);
}



int Sprite::getWidth() const {
	return _width;
}


int Sprite::getHeight() const {
	return _height;
}


// ---- SpriteSheet ----


SpriteSheet::SpriteSheet(std::vector<Rect> animations, Sprite* sprite)
: _animations(std::move(animations)), _sprite(sprite) {
	if (_animations.size() < 1) LOG("Animations vector contains no entry");
}

// set sprite to next sprite
void SpriteSheet::nextSprite() {
	_currentAnimation = ++_currentAnimation % (int)_animations.size();
}

// set sprite to previous sprite
void SpriteSheet::prevSprite() {
	--_currentAnimation;
	if (_currentAnimation < 0)
		_currentAnimation = (int)_animations.size() - 1;
}

// set the sprite index
void SpriteSheet::setSprite(int idx) {
	if (idx < (int)_animations.size())
		_currentAnimation = idx;
}

// return the spritesheet
Sprite*	SpriteSheet::getSprite() const {
	return _sprite;
}

// return the Rect showing the position of the sprite on the spritesheet
Rect*	SpriteSheet::getSpriteSrc() {
	return &_animations[_currentAnimation];
}