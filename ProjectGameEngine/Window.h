#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <string>
#include <SDL.h>

class Window {
private:
	SDL_Window* window = nullptr;

public:
	Window(std::string title, int width, int height) {
		if ((window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED,
			width, 
			height,
			SDL_WINDOW_SHOWN)) == nullptr) 
		{
			printf("Unable to create window: %s", SDL_GetError());
		}
	}

	~Window() {	SDL_DestroyWindow(window); }

	SDL_Window* getWindow() const { return window; }
};

#endif