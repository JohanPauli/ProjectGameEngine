#pragma once


#include <SDL.h>
#include <SDL_image.h>

#include <string>




// forward declarations
class Sprite;
class Rect;
class Point;


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
encapsulates SDL_Renderer
*/
class Renderer {
	friend class Window;
private:
	SDL_Renderer*	_renderer = nullptr;
	int				_xOffset = 0;
	int				_yOffset = 0;

private:
	// private constructor. Renderer only initialized from friend class
	Renderer(SDL_Renderer* renderer);
	Renderer(const Renderer&) = delete;
	void operator=(const Renderer&) = delete;
public:
	// deallocate the renderer
	~Renderer();

	/* 
		render a Sprite

		Sprite:	the Sprite to be rendered
		pos:		the position on screen to render the Sprite
		src:		what part of the Sprite should be rendered (optional)
		angle:		angle the Sprite should be rendered in (optional)
		center:		the center around which the Sprite should be turned (optional)
		flip:		flip the Sprite (optional)
	*/
	void render(
		Sprite*		sprite,
		Rect*		pos,
		Rect*		src = nullptr,
		double		angle = 0.0,
		Point*		center = nullptr,
		RenderFlip	flip = RenderFlip::NONE) const;

	void setOffsets(int x, int y);
	int  getXoffset() const;
	int  getYoffset() const;

	// loading functions
	Sprite* loadSprite(std::string path) const;
};




/*
	encapsulates SDL_Window
*/
class Window {
private:
	SDL_Window* _window;
	Renderer*	_renderer;
	int			_width;
	int			_height;

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
	int			getWidth() const;
	int			getHeight() const;
};