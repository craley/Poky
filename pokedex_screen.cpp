#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "options.hpp"
#include "render_context.hpp"
#include "imgui_sdlbackend.hpp"
#include "pokedex_screen.hpp"
#include "text.hpp"

using options::WINDOW_WIDTH;
using options::WINDOW_HEIGHT;

bool PokedexScreen::initialize(RenderContext *context)
{
	if (!Screen::initialize(context)) {
		return false;
	}

	m_pokeData.setPokemon("Butterfree");

	TTF_Font *m_font = TTF_OpenFont("assets/unifont-7.0.06.ttf", 16);
	if (m_font == nullptr) {
		std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		return false;
	}

	// Pokemon data
	{
		std::string txt = "Name: " + m_pokeData.getName()
			+ "\n" + "Base HP: " + std::to_string(m_pokeData.getBaseHP())
			+ "\n" + "Base Att: " + std::to_string(m_pokeData.getBaseAtt())
			+ "\n" + "Base Def: " + std::to_string(m_pokeData.getBaseDef())
			+ "\n" + "Base Sp Att: " + std::to_string(m_pokeData.getBaseSpAtt())
			+ "\n" + "Base Sp Def: " + std::to_string(m_pokeData.getBaseSpDef())
			+ "\n" + m_pokeData.getFlavorText();

		m_textTexture = getTextRenderTexture(m_context->renderer, m_font, txt);
		m_pokemonName.setImage(m_textTexture);
	}

	// Pokemon image
	{
		// TODO - sprites TBD
		// Butterfree for now
		m_pokemonImage = m_context->loadTexture("assets/sprites/Spr_5b_012.png");
		m_pokemonImage.setScale(3.0f);
		m_pokemonImage.setPosition(0, 100);
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
	m_context->render(m_pokemonImage);
	SDL_RenderPresent(m_context->renderer);
}
