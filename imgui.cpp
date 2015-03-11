#include <SDL.h>
#include <memory>
#include <iostream>
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
		keysEntered = "";
		m_currentKeyIndex = 0;
		scrollWheel = 0;
	}

	void UIState::handleEvent(const SDL_Event &sdlEvent)
	{
		switch(sdlEvent.type) {
			case SDL_MOUSEMOTION:
				mouseX = sdlEvent.motion.x;
				mouseY = sdlEvent.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
					mouseDown = true;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
					mouseDown = false;
				}
				break;
			case SDL_MOUSEWHEEL:
				scrollWheel += sdlEvent.wheel.y;
				break;
			case SDL_TEXTINPUT:
				keysEntered += sdlEvent.text.text;
				break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_BACKSPACE) {
					--m_currentKeyIndex;
				} else if (sdlEvent.key.keysym.sym == SDLK_v &&
						   (sdlEvent.key.keysym.mod == KMOD_LCTRL ||
							sdlEvent.key.keysym.mod == KMOD_RCTRL)) {
					keysEntered = SDL_GetClipboardText();
				}
				break;
			case SDL_TEXTEDITING:
				m_cursor = sdlEvent.edit.start;
				break;
		}
	}

	bool UIState::mouseHit(int x, int y, int w, int h)
	{
		if (mouseX < x || mouseY < y || mouseX >= x + w || mouseY >= y + h)
			return false;
		return true;
	}

	bool UIState::clickedSprite(int id, const Sprite &sprite) {
		if (mouseOverSprite(sprite)) {
			hotItem = id;
			if (activeItem == 0 && mouseDown) {
				activeItem = id;
			}
		}

		if (hotItem == id && activeItem == id && !mouseDown) {
			return true;
		}
		return false;
	}

	bool UIState::mouseOverSprite(const Sprite &sprite) {
		SDL_Rect rect = sprite.rect();
		return mouseHit(rect.x, rect.y, rect.w, rect.h);
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

	inline void popBackString(std::string &text) {
		while (text.size()) {
			char c = text.back();
			text.pop_back();
			// compatible with utf-8
			if ((c & 0xC0) != 0x80)
				break;
		}
	}

	bool UIState::textField(int id, int x, int y, int w, int h, std::string &text)
	{
		SDL_Color color = {128, 0, 128, 255};

		if (mouseHit(x, y, w, h)) {
			hotItem = id;
			if (activeItem == 0 && mouseDown) {
				activeItem = id;
			}
		}

		text += keysEntered;
		int index = m_currentKeyIndex;
		while (index < 0) {
			popBackString(text);
			index++;
		}

		const std::string tempString = text + std::string("|");

		SDL_Color white = {255, 255, 255, 255};
		m_renderBackend->drawRect(x, y, w, h, color);
		m_renderBackend->drawText(x, y, w, white, tempString);

		if (keysEntered.size()) {
			return true;
		}
		return false;
	}

	bool UIState::scrollBar(int id, int x, int y, int h, int max, float *val)
	{
		const SDL_Color color = {225, 0, 0, 255};
		const int width = 16, height = 24;
		int ypos = (h * (*val)) / max;

		if (mouseHit(x, y + ypos, width, height)) {
			hotItem = id;
			if (activeItem == 0 && mouseDown) {
				activeItem = id;
			}
		}

		m_renderBackend->drawRect(x, y + ypos, width, height, color);

		// todo only update based on scroll-wheel when window is active

		if (activeItem == id) {
			int mousePos = mouseY - y;
			if (mousePos < 0) {
				mousePos = 0;
			} else if (mousePos > h) {
				mousePos = h;
			}
			float tempValue = (mousePos*max) / h;
			if (*val != tempValue) {
				*val = tempValue;
				return true;
			}
		} else if (scrollWheel) {
			int tempValue = *val + (scrollWheel*max)/h;
			if (tempValue > max) {
				tempValue = max;
			} else if (tempValue < 0) {
				tempValue = 0;
			}
			*val = tempValue;
			return true;
		}

		return false;
	}

} // namespace imgui
