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
#include "sqlite3.h"

using options::WINDOW_WIDTH;
using options::WINDOW_HEIGHT;

int main(int, char**)
{
	RenderContext context;
	std::vector<std::unique_ptr<Screen>> screens;
	screens.emplace_back(new HomeScreen); // avoid copy

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

	while (!sdlQuit) {
		// Poll for events
		while(SDL_PollEvent(&sdlEvent)) {
			switch(sdlEvent.type) {
				case SDL_QUIT:
					sdlQuit = true;
					break;
			}

			screens[currentScreen]->handleEvent(sdlEvent);
		}

		screens[currentScreen]->frameStep(SDL_GetTicks());
	}

	return 0;
}
