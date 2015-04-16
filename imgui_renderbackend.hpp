#pragma once

#include <SDL.h>

namespace imgui {
	class RenderBackend {
	public:
		virtual void drawRect(int x, int y, int w, int h, SDL_Color color) = 0;
		virtual void drawText(int x, int y, int w, SDL_Color color, const std::string &text) = 0;
		virtual void drawTexture(int x, int y, int w, int h, SDL_Texture *texture) = 0;
		virtual void setRenderTarget(SDL_Texture *texture) = 0;
	};
} // namespace imgui
