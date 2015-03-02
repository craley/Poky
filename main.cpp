#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "options.hpp"
#include "render_context.hpp"
#include "home_screen.hpp"
#include "pokedex_screen.hpp"

int main(int, char**)
{
	RenderContext context;
	std::vector<std::unique_ptr<Screen>> screens;
	screens.emplace_back(new HomeScreen);
	screens.emplace_back(new PokedexScreen);

	if (!context.initalizeSDL()) {
		return 1;
	}
	for (auto &screen : screens) {
		if (!screen->initialize(&context)) {
			return 1;
		}
	}

	SDL_Event sdlEvent;
	bool sdlQuit = false;
	int currentScreen = 0;
	SDL_StartTextInput();

	while (!sdlQuit) {
		// Poll for events
		while(SDL_PollEvent(&sdlEvent)) {
			switch(sdlEvent.type) {
				case SDL_QUIT:
					sdlQuit = true;
					break;
				case SDL_KEYDOWN:
					if (sdlEvent.key.keysym.sym == SDLK_F1) {
						currentScreen = 0;
					} else if (sdlEvent.key.keysym.sym == SDLK_F2) {
						currentScreen = 1;
					}
					break;
			}

			screens[currentScreen]->handleEvent(sdlEvent);
		}

		screens[currentScreen]->frameStep(SDL_GetTicks());
	}

	return 0;
}
