#include <SDL.h>
#include <iostream>

int main( int argc, char ** argv )
{
	// Initialize SDL2.
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) != 0 )
	{
		std::cout << "SDL_Init error: " << SDL_GetError();
		SDL_Quit();
		return 1;
	}

	// Window pointer.
	SDL_Window *window;

	// Create window with title, position, size, and state.
	window = SDL_CreateWindow(
		"Template Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_SHOWN
	);

	// If window is not made, destroy window.
	if( window == nullptr )
	{
		std::cout << "SDL_CreateWindow error: " << SDL_GetError();
		SDL_Quit();
		return 1;
	}


	// Enter program loop / do something else.
	// E.g. wait for 5 seconds.
	SDL_Delay( 5000 );


	// Leave program when done.
	SDL_Quit();
	return 0;
}