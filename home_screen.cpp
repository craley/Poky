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

	// initialize the user interface
	m_userInterface.setRenderBackend(std::make_unique<imgui::SDLRenderBackend>(m_context->renderer));

	// Pokedex sprite
	{
		m_pokedexSprite.setImage(context->loadTexture("assets/pokedex.png"));
		m_pokedexSprite.setScale(5, 5);
		SDL_Rect dexRect = m_pokedexSprite.rect();
		m_pokedexSprite.setPosition(WINDOW_WIDTH/2 - dexRect.w/2, WINDOW_HEIGHT/3 - dexRect.h/2);
	}

	// Pokemon Snap Cartridge sprite
	{
		m_cartridgePokemonSnap.setImage(context->loadTexture("assets/cartridge.png"));
		m_cartridgePokemonSnap.setScale(2);
		SDL_Rect cartridgeRect = m_cartridgePokemonSnap.rect();
		SDL_Rect dexRect = m_pokedexSprite.rect();
		m_cartridgePokemonSnap.setPosition((dexRect.w - (2*cartridgeRect.w+10))/2 + dexRect.x,
				dexRect.y + dexRect.h + 15);
	}

	// Quiz Cartridge sprite
	{
		m_cartridgeQuiz.setImage(context->loadTexture("assets/cartridge.png"));
		m_cartridgeQuiz.setScale(2);
		SDL_Rect cartridgeRect = m_cartridgeQuiz.rect();
		SDL_Rect dexRect = m_pokedexSprite.rect();
		m_cartridgeQuiz.setPosition(m_cartridgePokemonSnap.rect().x + cartridgeRect.w + 10,
				dexRect.y + dexRect.h + 15);
	}

	// Pokeball sprite
	{
		SDL_Texture *pokeballTexture = context->loadTexture("assets/pokeball_background.png");
		SDL_SetTextureAlphaMod(pokeballTexture, 90);
		m_pokeball.setImage(pokeballTexture);
		m_pokeball.setScale(0.5f);
	}

	{
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
		SDL_Rect dexRect = m_pokedexSprite.rect();
		m_initialHeight = WINDOW_HEIGHT/2 + dexRect.h/2 + textWidth/2;
		m_textDest  = {WINDOW_WIDTH/2 - textWidth/2, m_initialHeight, textWidth, textHeight};
	}

	SDL_SetRenderDrawColor(context->renderer, 75, 67, 142, 255);

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
	SDL_DestroyTexture(m_cartridgePokemonSnap.texture());
	SDL_DestroyTexture(m_textTexture);
}

void HomeScreen::frameStep(unsigned long elapsedMS)
{
	static unsigned long lastTime = 0;
	float deltaS = static_cast<float>(elapsedMS - lastTime)/1000.0f;
	lastTime = elapsedMS;

	m_textDest.y = m_initialHeight + 5.0f*cos((float)elapsedMS/100.0f);

	SDL_RenderClear(m_context->renderer);

	// Render background
	{
		SDL_Rect ballRect = m_pokeball.rect();

		m_pokeball.setAngle(10.0f*cos(static_cast<float>(elapsedMS/500.0f)));
		for (int y = -1; y < WINDOW_HEIGHT/ballRect.h + 1; y++) {
			for (int x = -1; x < WINDOW_WIDTH/ballRect.w + 1; x++) { 
				float newX = static_cast<float>(x*ballRect.w);
				float newY = static_cast<float>(y*ballRect.h);
				m_pokeball.setPosition(newX, newY);
				m_context->render(m_pokeball);
			}
		}
	}

	if (m_dexDance) {
		m_pokedexSprite.setAngle(5.0f*cos(static_cast<float>(elapsedMS/100.0f)));
	} else if (m_pokedexSprite.angle() != 0) {
		float angle = m_pokedexSprite.angle();
		m_pokedexSprite.setAngle(angle + (0 - angle) * 10.0f * deltaS);
	}

	// Render image screen elements
	m_context->render(m_cartridgePokemonSnap);
	m_context->render(m_cartridgeQuiz);
	m_context->render(m_pokedexSprite);

	// Render the user interface
	m_userInterface.begin();
	if (m_userInterface.button(1, m_textDest.x, m_textDest.y, m_textDest.w, m_textDest.h)) {
		m_dexDance = !m_dexDance;
	}
	SDL_RenderCopy(m_context->renderer, m_textTexture, nullptr, &m_textDest);
	m_userInterface.end();

	SDL_RenderPresent(m_context->renderer);
}
