#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "screen.hpp"
#include "imgui.hpp"

class HomeScreen : Screen {
public:
	HomeScreen() :
		m_dexDance(false) {}

	bool initialize(RenderContext *context) override;
	void destroy() override;
	void handleEvent(SDL_Event &sdlEvent) override;
	void frameStep(unsigned long tickMS) override;
	void render(unsigned long tickMS) override;

private:
	bool m_dexDance;
	int m_initialHeight;

	SDL_Rect m_dexDest;
	SDL_Rect m_textDest;

	TTF_Font *m_font;

	SDL_Texture *m_dexTexture;
	SDL_Texture *m_textTexture;

	imgui::UIState m_userInterface;
};
