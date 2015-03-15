#include <SDL.h>
#include "imgui_sdlbackend.hpp"
#include "text.hpp"
#include "sprite.hpp"


namespace imgui {

	SDLRenderBackend::SDLRenderBackend(SDL_Renderer *renderer) :m_renderer(renderer)
	{
		m_font = TTF_OpenFont("assets/unifont-7.0.06.ttf", 16);
	}

	void SDLRenderBackend::drawRect(int x, int y, int w, int h, const SDL_Color color)
	{
		SDL_Rect rect = {x, y, w, h};
		uint8_t r, g, b, a;
		SDL_GetRenderDrawColor(m_renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(m_renderer, &rect);
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	}

	void SDLRenderBackend::drawText(int x, int y, int w, SDL_Color color, const std::string &text)
	{
		SDL_Texture *textTexture = getTextRenderTexture(m_renderer, m_font, text, color, w);
		SDL_Texture *textShadowTexture = getTextRenderTexture(m_renderer, m_font, text, {0, 0, 0, 255}, w);
		SDL_SetTextureAlphaMod(textShadowTexture, 90);


		// render the shadow
		{
			Sprite sprite(textShadowTexture);
			sprite.setPosition(x+2, y+2);
			SDL_Rect dest = sprite.rect();
			SDL_RenderCopyEx(m_renderer, textShadowTexture, nullptr, &dest,
					sprite.angle(), nullptr, SDL_FLIP_NONE);
		}

		// render the text
		{
			Sprite sprite(textTexture);
			sprite.setPosition(x, y);
			SDL_Rect dest = sprite.rect();
			SDL_RenderCopyEx(m_renderer, textTexture, nullptr, &dest,
					sprite.angle(), nullptr, SDL_FLIP_NONE);
		}

		// TODO: create texture cache
		SDL_DestroyTexture(textTexture);
		SDL_DestroyTexture(textShadowTexture);
	}

} // namespace imgui
