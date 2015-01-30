#pragma once

#include <SDL.h>

namespace imgui {
	class RenderBackend {
	public:
		virtual void drawRect(int x, int y, int w, int h, SDL_Color color) = 0;
	};
} // namespace imgui
