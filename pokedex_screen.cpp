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

	m_pokeData.setPokemon("Butterfree");

	// Pokemon name
	{
		TTF_Font *m_font = TTF_OpenFont("assets/unifont-7.0.06.ttf", 16);
		if (m_font == nullptr) {
			std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
			return false;
		}

		std::string wText = m_pokeData.getName();
		SDL_Surface *textSurface = TTF_RenderUNICODE_Solid(
				m_font,
				ext::UInt16String(wText.begin(), wText.end()).c_str(),
				SDL_Color({255, 255, 255, 255}));
		m_textTexture = SDL_CreateTextureFromSurface(m_context->renderer, textSurface);
		SDL_FreeSurface(textSurface);
		m_pokemonName.setImage(m_textTexture);
	}

	// Pokemon image
	{
		// TODO - sprites TBD
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
	m_context->render(m_pokemonName);
	SDL_RenderPresent(m_context->renderer);
}
