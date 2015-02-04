#include <iostream>
#include <cassert>
#include <SDL_image.h>
#include "sprite.hpp"

namespace surf {
	
	static inline void getMasks(Uint32 *gmask, Uint32 *rmask, Uint32 *bmask, Uint32 *amask)
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
	
	static inline void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
	{
		 // Convert the pixels to 32 bit
		 Uint32 *pixels = static_cast<Uint32 *>(surface->pixels);
		 // Set the pixel
		 pixels[( y * surface->w ) + x] = pixel;
	}

	static SDL_Surface *errorSurface(int w = 32, int h = 32)
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
} // namespace surf

Sprite::Sprite(SDL_Renderer *renderer, const std::string &imagePath)
{
	setImage(renderer, imagePath);
}

Sprite::Sprite(SDL_Texture *texture)
{
	setImage(texture);
}

void Sprite::setImage(SDL_Renderer *renderer, const std::string &imagePath)
{
	SDL_Surface *tempSurface = IMG_Load(imagePath.c_str());
	if (!tempSurface) {
		std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
		tempSurface = surf::errorSurface();
	}
	setImage(SDL_CreateTextureFromSurface(renderer, tempSurface));
	SDL_FreeSurface(tempSurface);
}

void Sprite::setImage(SDL_Texture *texture)
{
	m_texture = texture;
	SDL_QueryTexture(m_texture, nullptr, nullptr, &m_destRect.w, &m_destRect.h);
}

void Sprite::setPosition(const int x, const int y)
{
	m_destRect.x = x;
	m_destRect.y = y;
}

void Sprite::setScale(const float w, const float h)
{
	m_scaleX = w;
	m_scaleY = h;
}

void Sprite::setScale(const float s)
{
	setScale(s, s);
}

void Sprite::setSubRect(const int x, const int y, const int w, const int h)
{
	m_sourceRect = {x, y, w, h};
}

SDL_Rect Sprite::rect() const
{
	SDL_Rect scaled = m_destRect;
	scaled.w = m_destRect.w * m_scaleX;
	scaled.h = m_destRect.h * m_scaleY;

	return scaled;
}

SDL_Rect Sprite::subRect() const
{
	return m_sourceRect;
}

int Sprite::angle() const
{
	return m_angle;
}

SDL_Texture *Sprite::texture() const
{
	return m_texture;
}
