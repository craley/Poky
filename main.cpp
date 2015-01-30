#include <iostream>
#include <string>
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "options.hpp"
#include "render_context.hpp"
#include "imgui/imgui.hpp"
#include "imgui/imgui_sdlbackend.hpp"
#include "sqlite3.h"

int main(int, char**)
{
	using options::WINDOW_WIDTH;
	using options::WINDOW_HEIGHT;

	RenderContext context;
	if (!context.initalizeSDL()) {
		return 1;
	}

	SDL_Surface *dexSurface = IMG_Load("assets/pokedex.png");
	if (dexSurface == nullptr) {
		SDL_DestroyRenderer(context.renderer);
		SDL_DestroyWindow(context.window);
		std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
		return 1;
	}

	SDL_Texture *dexTexture = SDL_CreateTextureFromSurface(context.renderer, dexSurface);
	SDL_FreeSurface(dexSurface);
	if (dexTexture == nullptr) {
		SDL_DestroyTexture(dexTexture);
		SDL_DestroyRenderer(context.renderer);
		SDL_DestroyWindow(context.window);
		std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	TTF_Font *font;
	font = TTF_OpenFont("assets/DroidSansMono.ttf", 12);
	if (font == nullptr) {
		std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		SDL_DestroyRenderer(context.renderer);
		SDL_DestroyWindow(context.window);
		return 1;
	}

	//const SDL_Color white = {255, 255, 255, 255};
	const SDL_Color black = {0, 0, 0, 255};

	SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Click Me!", black);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(context.renderer, textSurface);
	SDL_FreeSurface(textSurface);

	int dexWidth, dexHeight;
	int textWidth, textHeight;

	SDL_QueryTexture(dexTexture, nullptr, nullptr, &dexWidth, &dexHeight);
	SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

	dexWidth *= 5; dexHeight *= 5;

	SDL_Rect dexDest = {WINDOW_WIDTH/2 - dexWidth/2, WINDOW_HEIGHT/2 - dexHeight/2, dexWidth, dexHeight};

	int initialHeight = WINDOW_HEIGHT/2 + dexHeight/2 + textWidth/2;
	SDL_Rect textDest = {WINDOW_WIDTH/2 - textWidth, initialHeight, 2*textWidth, 2*textHeight};
	bool dexDance = false;

	SDL_Event sdlEvent;
	bool sdlQuit = false;

	unsigned long timeElapsed = SDL_GetTicks();

	imgui::UIState userInterface;
	userInterface.setRenderBackend(
		std::make_unique<imgui::SDLRenderBackend>(context.renderer));

	while (!sdlQuit) {
		timeElapsed = SDL_GetTicks();

		// Poll for events
		while(SDL_PollEvent(&sdlEvent)) {
			switch(sdlEvent.type) {
				case SDL_QUIT:
					sdlQuit = true;
					break;
				case SDL_MOUSEMOTION:
					userInterface.mouseX = sdlEvent.motion.x;
					userInterface.mouseY = sdlEvent.motion.y;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
						userInterface.mouseDown = true;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (sdlEvent.button.button == SDL_BUTTON_LEFT)
						userInterface.mouseDown = false;
					break;
			}
		}

		textDest.y = initialHeight + 5.0f*cos((float)timeElapsed/100.0f);

		// Draw calls
		SDL_RenderClear(context.renderer);
		if (dexDance) {
			SDL_RenderCopyEx(
				context.renderer, dexTexture, nullptr, &dexDest,
				5*cos((float)timeElapsed/100.0f), nullptr, SDL_FLIP_NONE
			);
		} else {
			SDL_RenderCopy(context.renderer, dexTexture, nullptr, &dexDest);
		}

		userInterface.begin();
		if (userInterface.button(1, textDest.x, textDest.y, textDest.w, textDest.h)) {
			dexDance = !dexDance;
		}
		userInterface.end();

		SDL_RenderCopy(context.renderer, textTexture, nullptr, &textDest);
		SDL_RenderPresent(context.renderer);
	}

	SDL_DestroyTexture(dexTexture);
	SDL_DestroyTexture(textTexture);
	context.destroy();
	return 0;
}
