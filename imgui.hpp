#pragma once

#include <memory>

#include <SDL.h>
#include "imgui_renderbackend.hpp"
#include "sprite.hpp"

namespace imgui {
	class UIState {
	public:
		int mouseX = 0;
		int mouseY = 0;
		int hotItem = -1;
		int activeItem = -1;
		int scrollWheel = 0;
		bool mouseDown = false;
		std::string keysEntered;

		void begin();
		void end();

		void beginFrame(SDL_Texture *texture);
		void endFrame(SDL_Texture *texture);

		bool clickedSprite(int id, const Sprite &sprite);
		bool mouseOverSprite(const Sprite &sprite);
		bool mouseHit(int x, int y, int w, int h);
		bool button(int id, int x, int y, int w, int h);
		bool textField(int id, int x, int y, int w, int h, std::string &text);
		bool scrollBar(int id, int x, int y, int h, int max, float *val);
		void handleEvent(const SDL_Event &event);
		void setRenderBackend(std::unique_ptr<RenderBackend> &&renderBackend); 
	private:
		std::unique_ptr<RenderBackend> m_renderBackend = nullptr;
		int m_currentKeyIndex = 0;
		int m_cursor = 0;
	};
} // namespace ui
