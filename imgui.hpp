#pragma once

#include <memory>
#include "imgui_renderbackend.hpp"

namespace imgui {
	class UIState {
	public:
		int mouseX = 0;
		int mouseY = 0;
		int hotItem = -1;
		int activeItem = -1;
		bool mouseDown = false;
		std::string keysEntered;

		void begin();
		void end();

		bool mouseHit(int x, int y, int w, int h);
		bool button(int id, int x, int y, int w, int h);
		bool textField(int id, int x, int y, int w, int h, std::string &text);
		void handleEvent(const SDL_Event &event);
		void setRenderBackend(std::unique_ptr<RenderBackend> &&renderBackend); 
	private:
		std::unique_ptr<RenderBackend> m_renderBackend = nullptr;
		int m_currentKeyIndex = 0;
		int m_cursor = 0;
	};
} // namespace ui
