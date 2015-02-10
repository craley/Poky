#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "screen.hpp"
#include "imgui.hpp"
#include "sprite.hpp"
#include "PokemonData.hpp"

class PokedexScreen : public Screen {
public:
	~PokedexScreen();

	bool initialize(RenderContext *context) override;
	void handleEvent(const SDL_Event &sdlEvent) override;
	void frameStep(unsigned long tickMS) override;

private:
	imgui::UIState m_userInterface;
	SDL_Texture *m_textTexture;

	Sprite m_pokemonImage;
	Sprite m_pokemonName;

	PokemonData m_pokeData;
};
