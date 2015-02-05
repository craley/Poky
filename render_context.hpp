#pragma once

#include <SDL.h>
#include "sprite.hpp"

struct RenderContext {
	~RenderContext();
	SDL_Renderer *renderer = nullptr;
	SDL_Window *window = nullptr;

	bool initalizeSDL();
	void destroy();
	void render(const Sprite &sprite);
};
