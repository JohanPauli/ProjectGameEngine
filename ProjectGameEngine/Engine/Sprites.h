#pragma once


#include <SDL.h>
#include <SDL_image.h>

#include <vector>

// forward declarations
class Rect;
class Renderer;
enum class BlendMode;


/*
Encapsulates SDLs textures

Sprites can be loaded from the renderer class
*/
class Sprite {
	friend class Renderer;
private:
	SDL_Texture* _texture = nullptr;
	int _width = 0;
	int _height = 0;

public:
	Sprite(SDL_Texture* texture, int width, int height);
	~Sprite();

	// color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	// set blending
	void setBlendMode(BlendMode blending);

	// set alpha (opacity)
	void setAlpha(Uint8 alpha);

	// accessors
	int getWidth() const;
	int getHeight() const;
};




class SpriteSheet {
private:
	std::vector<Rect>	_animations;
	int					_currentAnimation = 0;
	Sprite*				_sprite = nullptr;

public:
	SpriteSheet(std::vector<Rect> animations, Sprite* sprite);

	// set sprite to next sprite
	void nextSprite();
	
	// set sprite to previous sprite
	void prevSprite();

	// set the sprite index
	void setSprite(int idx);

	// return the spritesheet
	Sprite*	getSprite() const;

	// return the Rect showing the position of the sprite on the spritesheet
	Rect*	getSpriteSrc();
};
