#include <iostream>
#include "texture.hpp"

void getMasks(Uint32 *gmask, Uint32 *rmask, Uint32 *bmask, Uint32 *amask)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	*rmask = 0xff000000;
	*gmask = 0x00ff0000;
	*bmask = 0x0000ff00;
	*amask = 0x000000ff;
#else
	*rmask = 0x000000ff;
	*gmask = 0x0000ff00;
	*bmask = 0x00ff0000;
	*amask = 0xff000000;
#endif
}

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	// Convert the pixels to 32 bit
	Uint32 *pixels = static_cast<Uint32 *>(surface->pixels);
	// Set the pixel
	pixels[( y * surface->w ) + x] = pixel;
}

SDL_Surface *errorSurface(int w, int h)
{
	Uint32 rmask, gmask, bmask, amask;
	// SDL interprets each pixel as a 32-bit number, so our masks must depend
	// on the endianness (byte order) of the machine
	getMasks(&rmask, &gmask, &bmask, &amask);

	SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
	if (!surface) {
		std::cerr << "SDL_CreateRGBSurface Error: " << SDL_GetError();
		exit(1);
	}

	if (SDL_MUSTLOCK(surface)) {
		SDL_LockSurface(surface);
	}

	Uint32 pixel = SDL_MapRGBA(surface->format, 255, 255, 255, 255);
	// rows
	for( int y = 0, ry = surface->h - 1; y < surface->h; y++, ry--) {
		// columns
		for(int x = 0, rx = surface->w - 1; x < surface->w; x++, rx--) {
			putPixel(surface, x, y, pixel);
		}
	}

	return surface;
}

SDL_Texture *loadTextureFromFile(SDL_Renderer *renderer, const std::string &imagePath)
{
	SDL_Surface *tempSurface = IMG_Load(imagePath.c_str());
	if (!tempSurface) {
		std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
		tempSurface = errorSurface();
	}
	SDL_FreeSurface(tempSurface);
	return SDL_CreateTextureFromSurface(renderer, tempSurface);
}
