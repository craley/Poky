#pragma once

#include <string>
#include <unordered_map>
#include <SDL.h>
#include "sprite.hpp"

struct RenderContext {
	~RenderContext();
	SDL_Renderer *renderer = nullptr;
	SDL_Window *window = nullptr;

	bool initalizeSDL();
	void destroy();
	void render(const Sprite &sprite);
	SDL_Texture *loadTexture(const std::string &texturePath);

private:
	std::unordered_map<std::string, SDL_Texture*> m_textureCache;
};
