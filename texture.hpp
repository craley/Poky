#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>

void getMasks(Uint32 *gmask, Uint32 *rmask, Uint32 *bmask, Uint32 *amask);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Surface *errorSurface(int w = 32, int h = 32);
SDL_Texture *loadTextureFromFile(SDL_Renderer *renderer, const std::string &imagePath);
