#include <iostream>
#include <cassert>
#include <SDL_image.h>
#include "sprite.hpp"
#include "texture.hpp"

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
	setImage(loadTextureFromFile(renderer, imagePath));
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

void Sprite::setAngle(float angle)
{
    m_angle = angle;
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

float Sprite::angle() const
{
	return m_angle;
}

SDL_Texture *Sprite::texture() const
{
	return m_texture;
}
