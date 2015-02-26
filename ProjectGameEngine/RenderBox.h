#ifndef __RENDER_BOX_H__
#define __RENDER_BOX_H__

#include <SDL.h>
#include <SDL_image.h>
#include "Window.h"
#include "Rect.h"



class Surface {
private:
	SDL_Surface* surface;

public:
	Surface(Window* window);
	Surface(std::string path);
	~Surface();
	
	SDL_Surface* getSurface() const;
};


class RenderBox {
private:
	Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Surface* windowSurface = nullptr;

public:
	RenderBox(Window* window);
	~RenderBox();

	bool init();
	bool isInitialized() const;


	// TODO: add rendering functions
	void update();
	void blit(Surface* surface, Rect* rect = nullptr);
};


#endif