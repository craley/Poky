#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "screen_dispatcher.hpp"
#include "options.hpp"
#include "render_context.hpp"
#include "home_screen.hpp"
#include "pokedex_screen.hpp"

int main(int, char**)
{
	ScreenDispatcher screenDispatcher;
	if (!screenDispatcher.initialize()) {
		return 1;
	}
	screenDispatcher.tick();
	return 0;
}
