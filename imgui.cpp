#include <SDL.h>
#include <memory>
#include "imgui.hpp"

namespace imgui {
	static const SDL_Color CYAN   = {0, 255, 255, 255};
	static const SDL_Color RED    = {255, 0, 0, 255};
	static const SDL_Color GREEN  = {0, 255, 0, 255};

	void UIState::begin()
	{
		hotItem = 0;
	}

	void UIState::end()
	{
		if (!mouseDown) {
			activeItem = 0;
		} else {
			if (activeItem == 0) {
				activeItem = -1;
			}
		}
	}

	bool UIState::mouseHit(int x, int y, int w, int h)
	{
		if (mouseX < x || mouseY < y || mouseX >= x + w || mouseY >= y + h)
			return false;
		return true;
	}

	void UIState::setRenderBackend(std::unique_ptr<RenderBackend> &&renderBackend)
	{
		m_renderBackend = std::move(renderBackend);
	}

	bool UIState::button(int id, int x, int y, int w, int h)
	{
		SDL_Color color = CYAN;

		if (mouseHit(x, y, w, h)) {
			hotItem = id;
			if (activeItem == 0 && mouseDown) {
				activeItem = id;
			}
		}
		if (hotItem == id) {
			if (activeItem == id) {
				color = GREEN;
			} else {
				color = RED;
			}
		}

		m_renderBackend->drawRect(x, y, w, h, color);

		if (hotItem == id && activeItem == id && !mouseDown) {
			return true;
		}

		return false;
	}
} // namespace ui
