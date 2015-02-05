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
	if (!Screen::initialize(context)) {
		return false;
	}

	m_cartridgeSprite.setImage(m_context->renderer, "assets/cartridge.png");
	m_cartridgeSprite.setScale(2);

	m_pokedexSprite.setImage(m_context->renderer, "assets/pokedex.png");
	m_pokedexSprite.setScale(5, 5);
	SDL_Rect dexRect = m_pokedexSprite.rect();
	m_pokedexSprite.setPosition(WINDOW_WIDTH/2 - dexRect.w/2, WINDOW_HEIGHT/2 - dexRect.h/2);

	m_font = TTF_OpenFont("assets/unifont-7.0.06.ttf", 16);
	if (m_font == nullptr) {
		std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		return false;
	}

	std::wstring wText = L"Female: \u2640, Male: \u2642";

	SDL_Surface *textSurface = TTF_RenderUNICODE_Solid(
		m_font,
		ext::UInt16String(wText.begin(), wText.end()).c_str(),
		SDL_Color({0, 0, 0, 255}));
	m_textTexture = SDL_CreateTextureFromSurface(m_context->renderer, textSurface);
	SDL_FreeSurface(textSurface);

	int textWidth, textHeight;
	SDL_QueryTexture(m_textTexture, nullptr, nullptr, &textWidth, &textHeight);
	m_initialHeight = WINDOW_HEIGHT/2 + dexRect.h/2 + textWidth/2;
	m_textDest  = {WINDOW_WIDTH/2 - textWidth/2, m_initialHeight, textWidth, textHeight};

	m_userInterface.setRenderBackend(std::make_unique<imgui::SDLRenderBackend>(m_context->renderer));

	return true;
}

void HomeScreen::handleEvent(const SDL_Event &sdlEvent)
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

HomeScreen::~HomeScreen()
{
	SDL_DestroyTexture(m_pokedexSprite.texture());
	SDL_DestroyTexture(m_cartridgeSprite.texture());
	SDL_DestroyTexture(m_textTexture);
}

void HomeScreen::frameStep(unsigned long elapsedMS)
{
	m_textDest.y = m_initialHeight + 5.0f*cos((float)elapsedMS/100.0f);

	SDL_RenderClear(m_context->renderer);

	if (m_dexDance) {
        m_pokedexSprite.setAngle(5.0f*cos(static_cast<float>(elapsedMS/100.0f)));
	} else if (m_pokedexSprite.angle() != 0) {
        int angle = m_pokedexSprite.angle();
        if (angle < 0) {
            m_pokedexSprite.setAngle(1.0f*(elapsedMS/1000.0f)+angle);
        } else if (angle > 0) {
            m_pokedexSprite.setAngle(-1.0f*(elapsedMS/1000.0f)+angle);
        }
    }

	m_userInterface.begin();
	if (m_userInterface.button(1, m_textDest.x, m_textDest.y, m_textDest.w, m_textDest.h)) {
		m_dexDance = !m_dexDance;
	}
	m_userInterface.end();

	SDL_RenderCopy(m_context->renderer, m_textTexture, nullptr, &m_textDest);
	m_context->render(m_cartridgeSprite);
	m_context->render(m_pokedexSprite);
	SDL_RenderPresent(m_context->renderer);
}
