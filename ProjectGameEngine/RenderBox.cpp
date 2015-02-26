#include "RenderBox.h"


// ---- Surface ----

Surface::Surface(Window* window) {
	if ((surface = SDL_GetWindowSurface(window->getWindow())) == nullptr) {
		printf("Unable to get surface from window. SDL error: %s",  SDL_GetError());
	}
}


Surface::Surface(std::string path) {
	if ((surface = IMG_Load(path.c_str())) == nullptr) {
		printf("Unable to load image \"%s\". SDL error: %s", path, SDL_GetError());
	}
}


Surface::~Surface() {
	SDL_FreeSurface(surface);
}


SDL_Surface* Surface::getSurface() const {
	return surface;
}



// ---- RenderBox ----

RenderBox::RenderBox(Window* window) 
	: window(window) {}


RenderBox::~RenderBox() { 
	SDL_DestroyRenderer(renderer); 
}


bool RenderBox::init() {
	if ((renderer = SDL_CreateRenderer(window->getWindow(), -1, SDL_RENDERER_ACCELERATED)) == nullptr) {
		printf("Unable to create renderer: %s", SDL_GetError());
		return false;
	}
	if ((windowSurface = new Surface(window)) == nullptr) {
		return false;
	}

	return true;
}


bool RenderBox::isInitialized() const { 
	return (renderer != nullptr)
		&& (windowSurface != nullptr); 
}


void RenderBox::update() {
	SDL_UpdateWindowSurface(window->getWindow());
}


void RenderBox::blit(Surface* surface, Rect* rect) {
	SDL_BlitSurface(surface->getSurface(),
		nullptr,
		windowSurface->getSurface(),
		rect->getRect());
}