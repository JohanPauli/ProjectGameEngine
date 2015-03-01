#include "Rendering.h"

#include "Debug.h"






// ---- Texture ----

Texture::Texture(SDL_Texture* texture, int width, int height) 
	: _texture(texture), _width(width), _height(height) {}


Texture::~Texture() {
	SDL_DestroyTexture(_texture);
}


// color modulation
void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(_texture, red, green, blue);
}


// set blending
void Texture::setBlendMode(BlendMode blending) {
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
void Texture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(_texture, alpha);
}



int Texture::getWidth() const {
	return _width;
}


int Texture::getHeight() const {
	return _height;
}


SDL_Texture* Texture::getTexture() const {
	return _texture;
}




// ---- Renderer ----

Renderer::Renderer(SDL_Renderer* renderer)
: _renderer(renderer) {}

Renderer::~Renderer() {
	SDL_DestroyRenderer(_renderer);
}
/*
render texture at a given point
angle: the rotation of the object in degrees
point: the point to rotate around
flip: whether to flip the sprite texture or not
*/
void Renderer::render(Texture* texture, Rect* pos, Rect* texPos, double angle, Point* center, RenderFlip flip) const {
	assert(texture != nullptr); assert(pos != nullptr);
	SDL_RendererFlip sdlFlip;
	switch (flip) {
		case RenderFlip::NONE:		sdlFlip = SDL_FLIP_NONE; break;
		case RenderFlip::HORIZONTAL:sdlFlip = SDL_FLIP_HORIZONTAL; break;
		case RenderFlip::VERTICAL:	sdlFlip = SDL_FLIP_VERTICAL; break;
	}
	SDL_RenderCopyEx(
		_renderer,
		texture->getTexture(),
		texPos->getRect(),
		pos->getRect(),
		angle,
		center->getPoint(),
		sdlFlip);
}


Texture* Renderer::loadTexture(std::string path) const {
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
	auto texture = new Texture(sdlTexture, sdlSurface->w, sdlSurface->h);

	SDL_FreeSurface(sdlSurface);
	return texture;
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
