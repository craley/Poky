#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "options.hpp"

inline SDL_Texture *getTextRenderTexture(SDL_Renderer *renderer, TTF_Font *font,
		const std::string &str, const SDL_Color color={255, 255, 255, 255},
		const uint32_t width=options::WINDOW_WIDTH)
{
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(
			font,
			str.c_str(),
			color,
			width);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	return texture;
}
