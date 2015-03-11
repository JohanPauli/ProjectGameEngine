#include "Rendering.h"

#include "Debug.h"
#include "Sprites.h"
#include "Rect.h"





// ---- Renderer ----

Renderer::Renderer(SDL_Renderer* renderer)
: _renderer(renderer) {}

Renderer::~Renderer() {
	SDL_DestroyRenderer(_renderer);
}
/*
render Sprite at a given point
angle: the rotation of the object in degrees
point: the point to rotate around
flip: whether to flip the sprite Sprite or not
*/
void Renderer::render(Sprite* sprite, Rect* pos, Rect* src, double angle, Point* center, RenderFlip flip) const {
	SDL_RendererFlip sdlFlip;
	switch (flip) {
		case RenderFlip::NONE:		sdlFlip = SDL_FLIP_NONE; break;
		case RenderFlip::HORIZONTAL:sdlFlip = SDL_FLIP_HORIZONTAL; break;
		case RenderFlip::VERTICAL:	sdlFlip = SDL_FLIP_VERTICAL; break;
	}
	SDL_Rect offset = pos->rect;
	offset.x -= _xOffset;
	offset.y -= _yOffset;

	SDL_RenderCopyEx(
		_renderer,
		sprite->_texture,
		&src->rect,
		&offset,
		angle,
		&center->point,
		sdlFlip);
}


void Renderer::setOffsets(int x, int y) {
	_xOffset = x;
	_yOffset = y;
}


Sprite* Renderer::loadSprite(std::string path) const {
	SDL_Texture* sdlTexture;
	SDL_Surface* sdlSurface;
	if ((sdlSurface = IMG_Load(path.c_str())) == nullptr) {
		LOG("Unable to load image \"" << path << "\". SDL_image error: " << IMG_GetError());
		return nullptr;
	}
	if ((sdlTexture = SDL_CreateTextureFromSurface(_renderer, sdlSurface)) == nullptr) {
		LOG("Unable to create texture from image \"" << path << "\". SDL error: " << SDL_GetError());
		return nullptr;
	}
	auto sprite = new Sprite(sdlTexture, sdlSurface->w, sdlSurface->h);

	SDL_FreeSurface(sdlSurface);
	return sprite;
}







// ---- Window ----


Window::Window(std::string title, int width, int height) {
	// initialize sdl video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG("SDL_Init failed on SDL_INIT_VIDEO: " << SDL_GetError());
	}

	// initialize window
	if ((_window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN)) == nullptr)
	{
		LOG("Unable to create window: " << SDL_GetError());
	}

	// initialize renderer
	SDL_Renderer* renderer;
	if ((renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED)) == nullptr)
		LOG("Unable to create renderer: " << SDL_GetError());
	else
		_renderer = new Renderer(renderer);

	// set width and height variables
	SDL_GetWindowSize(_window, &_width, &_height);
}

Window::~Window() {
	SDL_DestroyWindow(_window);
	delete _renderer;
}

/*
	draw the window
*/
void Window::update() const {
	SDL_RenderPresent(_renderer->_renderer);
	SDL_UpdateWindowSurface(_window);
}


Renderer* Window::getRenderer() const {
	return _renderer;
}


int Window::getWidth() const {
	return _width;
}


int Window::getHeight() const {
	return _height;
}