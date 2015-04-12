#include "screen_dispatcher.hpp"
#include "render_context.hpp"
#include "pokedex_screen.hpp"
#include "home_screen.hpp"
#include "snap_screen.hpp"

bool ScreenDispatcher::initialize()
{
	m_screens.emplace_back(new HomeScreen);
	m_screens.emplace_back(new PokedexScreen);
	m_screens.emplace_back(new SnapScreen);

	if (!m_context.initalizeSDL()) {
		return false;
	}
	for (auto &screen : m_screens) {
		if (!screen->initialize(&m_context, this)) {
			return false;
		}
	}

	SDL_StartTextInput();

	return true;
}
	
void ScreenDispatcher::setToPokedexScreen()
{
	m_currentScreen = POKEDEXSCREEN;
}

void ScreenDispatcher::tick()
{
	SDL_Event sdlEvent;
	while (!m_sdlQuit) {
		// Poll for events
		while(SDL_PollEvent(&sdlEvent)) {
			switch(sdlEvent.type) {
				case SDL_QUIT:
					m_sdlQuit = true;
					break;
				case SDL_KEYDOWN:
					if (sdlEvent.key.keysym.sym == SDLK_F1) {
						m_currentScreen = 0;
					} else if (sdlEvent.key.keysym.sym == SDLK_F2) {
						m_currentScreen = 1;
					} else if (sdlEvent.key.keysym.sym == SDLK_F3) {
						m_currentScreen = 2;
					}

					break;
			}

			m_screens[m_currentScreen]->handleEvent(sdlEvent);
		}

		m_screens[m_currentScreen]->frameStep(SDL_GetTicks());
	}
}
