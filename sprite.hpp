#pragma once

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>


#define EMPTY_RECT {0, 0, 0, 0}

class Sprite {
public:
	Sprite() {};
	Sprite(SDL_Renderer *renderer, const std::string &imagePath);
	Sprite(SDL_Texture *texture);

	void setImage(SDL_Renderer *renderer, const std::string &imagePath);
	void setImage(SDL_Texture *texture);

	void setPosition(const int x, const int y);
	void setScale(const float w, const float h);
	void setScale(const float s);
	void setSubRect(const int x, const int y, const int w, const int h);
	void setAngle(float angle);

	SDL_Rect rect() const;
	SDL_Rect subRect() const;
	float angle() const;
	SDL_Texture *texture() const;

private:
	float m_angle = 0.0f;
	float m_scaleX = 1.0f;
	float m_scaleY = 1.0f;
	SDL_Rect m_destRect = EMPTY_RECT;
	SDL_Rect m_sourceRect = EMPTY_RECT;
	SDL_Texture *m_texture = nullptr;
};
