#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "render_context.hpp"
#include "options.hpp"
#include "home_screen.hpp"
#include "imgui_sdlbackend.hpp"

using options::WINDOW_WIDTH;
using options::WINDOW_HEIGHT;

namespace ext {
	typedef std::basic_string<Uint16, std::char_traits<Uint16>, std::allocator<Uint16>> UInt16String;
}

bool HomeScreen::initialize(RenderContext *context)
{
	Screen::initialize(context);

	SDL_Surface *dexSurface = IMG_Load("assets/pokedex.png");
	if (dexSurface == nullptr) {
		std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
		return false;
	}

	m_dexTexture = SDL_CreateTextureFromSurface(m_context->renderer, dexSurface);
	SDL_FreeSurface(dexSurface);
	if (m_dexTexture == nullptr) {
		SDL_DestroyTexture(m_dexTexture);
		std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_font = TTF_OpenFont("assets/unifont-7.0.06.ttf", 16);
	if (m_font == nullptr) {
		std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		return false;
	}

	//const SDL_Color white = {255, 255, 255, 255};
	const SDL_Color black = {0, 0, 0, 255};
	std::wstring wText = L"Female: \u2640, Male: \u2642";

	SDL_Surface *textSurface = TTF_RenderUNICODE_Solid(
		m_font,
		ext::UInt16String(wText.begin(), wText.end()).c_str(),
		black);
	m_textTexture = SDL_CreateTextureFromSurface(m_context->renderer, textSurface);
	SDL_FreeSurface(textSurface);

	int dexWidth, dexHeight;
	int textWidth, textHeight;

	SDL_QueryTexture(m_dexTexture, nullptr, nullptr, &dexWidth, &dexHeight);
	SDL_QueryTexture(m_textTexture, nullptr, nullptr, &textWidth, &textHeight);

	dexWidth *= 5; dexHeight *= 5;
	m_dexDest = {
		WINDOW_WIDTH/2 - dexWidth/2,
		WINDOW_HEIGHT/2 - dexHeight/2,
		dexWidth, dexHeight
	};

	m_initialHeight = WINDOW_HEIGHT/2 + dexHeight/2 + textWidth/2;
	m_textDest  = {WINDOW_WIDTH/2 - textWidth/2, m_initialHeight, textWidth, textHeight};

	m_userInterface.setRenderBackend(std::make_unique<imgui::SDLRenderBackend>(m_context->renderer));

	return true;
}

void HomeScreen::frameStep(unsigned long elapsedMS)
{
	m_textDest.y = m_initialHeight + 5.0f*cos((float)elapsedMS/100.0f);
}

void HomeScreen::handleEvent(SDL_Event &sdlEvent)
{
	switch(sdlEvent.type) {
		case SDL_MOUSEMOTION:
			m_userInterface.mouseX = sdlEvent.motion.x;
			m_userInterface.mouseY = sdlEvent.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
				m_userInterface.mouseDown = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (sdlEvent.button.button == SDL_BUTTON_LEFT)
				m_userInterface.mouseDown = false;
			break;
	}
}

void HomeScreen::destroy()
{
	SDL_DestroyTexture(m_dexTexture);
	SDL_DestroyTexture(m_textTexture);
}

void HomeScreen::render(unsigned long elapsedMS)
{
	SDL_RenderClear(m_context->renderer);

	if (m_dexDance) {
		// Give the object some oscillation
		SDL_RenderCopyEx(
				m_context->renderer, m_dexTexture, nullptr, &m_dexDest,
				5*cos((float)elapsedMS/100.0f), nullptr, SDL_FLIP_NONE
				);
	} else {
		SDL_RenderCopy(m_context->renderer, m_dexTexture, nullptr, &m_dexDest);
	}

	m_userInterface.begin();
	if (m_userInterface.button(1, m_textDest.x, m_textDest.y, m_textDest.w, m_textDest.h)) {
		m_dexDance = !m_dexDance;
	}
	m_userInterface.end();

	SDL_RenderCopy(m_context->renderer, m_textTexture, nullptr, &m_textDest);
	SDL_RenderPresent(m_context->renderer);
}
