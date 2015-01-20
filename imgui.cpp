#include <SDL2/SDL.h>
#include "imgui.h"

namespace ui {
	static const SDL_Color CYAN = {0, 255, 255};

	void drawRect(SDL_Renderer *renderer, int x, int y, int w, int h, const SDL_Color color)
	{
		SDL_Rect rect = {x, y, w, h};
		uint8_t r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	bool doButton(int id, SDL_Renderer *renderer, int x, int y, int w, int h)
	{
		drawRect(renderer, x, y, w, h, CYAN);
		return false;
	}
} // namespace ui
