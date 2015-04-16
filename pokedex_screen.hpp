#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "screen.hpp"
#include "imgui.hpp"
#include "sprite.hpp"
#include "PokemonData.hpp"

#include <Gwen/Utility.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Input/SDL2.h>
#include <Gwen/Renderers/SDL2.h>

class Pokedex;
class PokemonList;

class PokedexScreen : public Screen {
public:
	~PokedexScreen();

	bool initialize(RenderContext *context, ScreenDispatcher *dispatcher) override;
	void handleEvent(const SDL_Event &sdlEvent) override;
	void frameStep(unsigned long tickMS) override;
	void setPokedexData(int id);

private:
	PokemonData m_pokeData;
	std::string m_textField;

	Sprite m_pokemonImage;
	Sprite m_pokemonName;
	Sprite m_window;
	Sprite m_textTest;

	int m_currentPokemonID;
	int m_totalPokemon = 0;

	// GWEN Stuff
	Gwen::Renderer::SDL2 *m_gwenRenderer = nullptr;
	Gwen::Controls::Canvas *m_gwenCanvas = nullptr;
	Gwen::Skin::TexturedBase *m_gwenSkin = nullptr;
	Pokedex *m_pokedexBase= nullptr;

    Gwen::Input::SDL2 m_gwenInput;
};
