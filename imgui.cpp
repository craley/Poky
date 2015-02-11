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
		memset(keysEntered, 0, sizeof(keysEntered));
		m_currentKeyIndex = 0;
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
				if (sdlEvent.button.button == SDL_BUTTON_LEFT)
					mouseDown = false;
				break;
			case SDL_TEXTINPUT:
				strcat(keysEntered, sdlEvent.text.text);
				++m_currentKeyIndex;
				break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_BACKSPACE) {
				--m_currentKeyIndex;
				}
				break;
			case SDL_TEXTEDITING:
				m_cursor = sdlEvent.edit.start;
				std::cout << "cursor: " << m_cursor << std::endl;
				break;
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

	bool UIState::textField(int id, int x, int y, int w, int h, std::string &text)
	{
		SDL_Color color = {128, 0, 128, 80};

		if (mouseHit(x, y, w, h)) {
			hotItem = id;
			if (activeItem == 0 && mouseDown) {
				activeItem = id;
			}
		}

		std::string keysStr = std::string(keysEntered);
		text += keysStr;
		if (m_currentKeyIndex < 0) {
			int index = m_currentKeyIndex;
			while (index && text.size()) {
				text.pop_back();
				index++;
			}
		}
		const std::string tempString = text + std::string("|");

		SDL_Color white = {255, 255, 255, 255};
		m_renderBackend->drawRect(x, y, w, h, color);
		m_renderBackend->drawText(x, y, w, white, tempString);

		if (keysStr.size()) {
			return true;
		}
		return false;
	}
} // namespace imgui
