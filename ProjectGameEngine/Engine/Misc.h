
#pragma once

#include <SDL.h>


namespace Flappy {
	/*
	captures SDL_QUIT events
	*/
	void pollEvents(bool& running) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_QUIT:
				running = false;
				break;
			default:break;
			}
		}
	}

	void delay(Uint32 ms) {
		SDL_Delay(ms);
	}

	void quit(){
		SDL_Quit();
	}
}