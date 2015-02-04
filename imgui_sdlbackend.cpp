#include <SDL.h>
#include "imgui_sdlbackend.hpp"


namespace imgui {

	void SDLRenderBackend::drawRect(int x, int y, int w, int h, const SDL_Color color)
	{
		SDL_Rect rect = {x, y, w, h};
		uint8_t r, g, b, a;
		SDL_GetRenderDrawColor(m_renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(m_renderer, &rect);
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	}

} // namespace imgui
