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

Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
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
	SDL_Texture *createdTexture= SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return createdTexture;
}
