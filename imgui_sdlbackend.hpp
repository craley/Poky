#include <SDL.h>
#include "imgui_renderbackend.hpp"

namespace imgui {
	class SDLRenderBackend :public RenderBackend {
	public:
		SDLRenderBackend(SDL_Renderer *renderer) :m_renderer(renderer) {}
		void drawRect(int x, int y, int w, int h, SDL_Color color) override;
	private:
		SDL_Renderer *m_renderer;
	};
}
