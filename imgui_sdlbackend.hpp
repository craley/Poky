#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "imgui_renderbackend.hpp"

namespace imgui {
	class SDLRenderBackend :public RenderBackend {
	public:
		SDLRenderBackend(SDL_Renderer *renderer);
		void drawRect(int x, int y, int w, int h, SDL_Color color) override;
		void drawText(int x, int y, int w, SDL_Color color, const std::string &text) override;
	private:
		SDL_Renderer *m_renderer;
		TTF_Font *m_font;
	};
}
