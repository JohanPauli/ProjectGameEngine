#ifndef __RENDERING_H__
#define __RENDERING_H__

#include <SDL.h>
#include <SDL_image.h>

#include <string>

#include "Rect.h"


// various renderer related enumerators

enum class BlendMode {
	NONE,
	BLEND,
	ADD,
	MOD
};

enum class RenderFlip {
	NONE,
	HORIZONTAL,
	VERTICAL,
};




/*
	Encapsulates SDLs textures

	Textures can be loaded from the renderer class 
*/
class Texture {
private:
	SDL_Texture* _texture = nullptr;
	int _width = 0;
	int _height = 0;

public:
	Texture(SDL_Texture* texture, int width, int height);
	~Texture();

	// color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	// set blending
	void setBlendMode(BlendMode blending);

	// set alpha (opacity)
	void setAlpha(Uint8 alpha);

	// accessors
	int getWidth() const;
	int getHeight() const;
	SDL_Texture* getTexture() const;
};




/*
encapsulates SDL_Renderer
*/
class Renderer {
	friend class Window;
private:
	SDL_Renderer*	_renderer = nullptr;

	// private constructor. Renderer only initialized from friend class
	Renderer(SDL_Renderer* renderer);
	Renderer(const Renderer&) = delete;
	void operator=(const Renderer&) = delete;
public:
	// deallocate the renderer
	~Renderer();

	/* 
		render a texture

		texture:	the texture to be rendered
		pos:		the position on screen to render the texture
		texPos:		what part of the texture should be rendered (optional)
		angle:		angle the texture should be rendered in (optional)
		center:		the center around which the texture should be turned (optional)
		flip:		flip the texture (optional)
	*/
	void render(
		Texture*	texture,
		Rect*		pos,
		Rect*		texPos = nullptr,
		double		angle = 0.0,
		Point*		center = nullptr,
		RenderFlip	flip = RenderFlip::NONE) const;

	// loading functions
	Texture* loadTexture(std::string path) const;
};




/*
	encapsulates SDL_Window
*/
class Window {
private:
	SDL_Window* _window = nullptr;
	Renderer*	_renderer = nullptr;

public:
	Window(std::string title, int width, int height);
	// deallocation
	~Window();

	// draw the window
	void		update() const;

	/* 
		access the window's renderer
		may return nullptr if renderer failed to initialize
	*/
	Renderer*	getRenderer() const;
};





#endif