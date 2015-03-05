
#pragma once

#include <SDL.h>
#include "InputMapping.h"

// More event types than necessary
/*enum EventType {
	QUIT = SDL_QUIT,
	KEYDOWN = SDL_KEYDOWN,
	KEYUP = SDL_KEYUP,
	MOUSE_BTN_DOWN = SDL_MOUSEBUTTONDOWN,
	MOUSE_BTN_UP = SDL_MOUSEBUTTONUP,
	MOUSE_WHEEL = SDL_MOUSEWHEEL,
	MOUSE_MOTION = SDL_MOUSEMOTION,
	DROP = SDL_DROPFILE,
	WINDOW = SDL_WINDOWEVENT,
	TEXT_EDIT = SDL_TEXTEDITING,
	TEXT_INPUT = SDL_TEXTINPUT
};*/

namespace InputHandling {
/*
encapsulates SDL_Event and its related functions
*/
void pollInput(InputMapper* inputMapper, bool& running) {
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			inputMapper->notify(static_cast<KeyboardInput>(ev.key.keysym.sym));
			break;
		default:break;
		}
	}
}
}

