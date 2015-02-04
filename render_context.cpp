#include <iostream>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "render_context.hpp"
#include "options.hpp"

bool RenderContext::initalizeSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF) == -1) {
		std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() == -1) {
		std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow(
			"Pokedex",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			options::WINDOW_WIDTH,
			options::WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(
			window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	return true;
}

void RenderContext::destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void RenderContext::render(const Sprite &sprite)
{
	SDL_Rect dest = sprite.rect();
	SDL_RenderCopyEx(renderer, sprite.texture(), nullptr, &dest,
					sprite.angle(), nullptr, SDL_FLIP_NONE);
}
