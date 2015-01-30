#pragma once

#include <SDL.h>

struct RenderContext {
	SDL_Renderer *renderer;
	SDL_Window *window;

	bool initalizeSDL();
	void destroy();
};
