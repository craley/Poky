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

enum class ScreenType { HOME };

int main(int, char**)
{
	RenderContext context;

	HomeScreen homeScreen;

	if (!context.initalizeSDL()) {
		context.destroy();
		return 1;
	}
	if (!homeScreen.initialize(&context)) {
		context.destroy();
		return 1;
	}

	SDL_Event sdlEvent;
	bool sdlQuit = false;
	ScreenType currentScreen = ScreenType::HOME;

	while (!sdlQuit) {
		// Poll for events
		while(SDL_PollEvent(&sdlEvent)) {
			switch(sdlEvent.type) {
				case SDL_QUIT:
					sdlQuit = true;
					break;
			}

			switch(currentScreen) {
				case ScreenType::HOME:
					homeScreen.handleEvent(sdlEvent);
					break;
			}
		}

		auto timeElapsed = SDL_GetTicks();

		switch(currentScreen) {
			case ScreenType::HOME:
				homeScreen.frameStep(timeElapsed);
				break;
		}

		switch(currentScreen) {
			case ScreenType::HOME:
				homeScreen.render(timeElapsed);
				break;
		}
	}

	homeScreen.destroy();
	context.destroy();
	return 0;
}
