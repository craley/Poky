#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "options.hpp"
#include "render_context.hpp"
#include "imgui_sdlbackend.hpp"
#include "pokedex_screen.hpp"

using options::WINDOW_WIDTH;
using options::WINDOW_HEIGHT;

namespace ext {
	typedef std::basic_string<Uint16, std::char_traits<Uint16>, std::allocator<Uint16>> UInt16String;
}

bool PokedexScreen::initialize(RenderContext *context)
{
	if (!Screen::initialize(context)) {
		return false;
	}

	// initialize the user interface
	m_userInterface.setRenderBackend(std::make_unique<imgui::SDLRenderBackend>(m_context->renderer));
	return true;
}

void PokedexScreen::handleEvent(const SDL_Event &sdlEvent)
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

PokedexScreen::~PokedexScreen()
{
}

void PokedexScreen::frameStep(unsigned long elapsedMS)
{
	SDL_RenderClear(m_context->renderer);
	SDL_RenderPresent(m_context->renderer);
}
