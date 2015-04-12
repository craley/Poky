
#include "snap_screen.hpp"

#include <string>
#include <iostream>
#include <random>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "options.hpp"
#include "render_context.hpp"
#include "imgui_sdlbackend.hpp"
#include "text.hpp"

using options::WINDOW_WIDTH;
using options::WINDOW_HEIGHT;

SnapScreen::SnapScreen() {

}

SnapScreen::~SnapScreen() {

}

bool SnapScreen::initialize(RenderContext *context, ScreenDispatcher *dispatcher) {
	if (!Screen::initialize(context, dispatcher)) {
		return false;
	}

	// initialize the user interface
	m_userInterface.setRenderBackend(std::make_unique<imgui::SDLRenderBackend>(m_context->renderer));

	time = std::clock();
	countdown = to_millis(3, 0);
	//mouse cursor
	cursor.mx = WINDOW_WIDTH * 0.5;
	cursor.my = WINDOW_HEIGHT * 0.5;

	font = TTF_OpenFont("assets/unifont-7.0.06.ttf", 16);
	timerText = std::to_string(countdown);
	timerTexture = getTextRenderTexture(m_context->renderer, font, timerText, SDL_Color {
		0, 0, 0, 255
	});

	int textWidth, textHeight;
	//dump texture attribs into width and height
	SDL_QueryTexture(timerTexture, nullptr, nullptr, &textWidth, &textHeight);
	SDL_Rect dexRect = background.rect();
	timerRect = {5, WINDOW_WIDTH - textWidth - 10, textWidth, textHeight};

	background.setImage(context->loadTexture("assets/pokyBackground.jpg"));
	background.setPosition(0, 0);
	background.setScale(WINDOW_WIDTH, WINDOW_HEIGHT);


	return true;
}

void SnapScreen::handleEvent(const SDL_Event &sdlEvent) {
	switch (sdlEvent.type) {

	}
}

void SnapScreen::frameStep(unsigned long) {
	std::clock_t now = std::clock();
	std::clock_t elapsed = now - time;
	if ((countdown -= elapsed) < 0) {
		//game over
	}
	time = now;
	timerText = std::to_string(countdown);

	//rendering::

	//clear the current target
	SDL_RenderClear(m_context->renderer);

	m_context->render(background);

	//copy portion of texture to render target
	SDL_RenderCopy(m_context->renderer, timerTexture, nullptr, &timerRect);
	//m_userInterface.end();

	//update screen with any rendering performed since last update
	SDL_RenderPresent(m_context->renderer);
}

void SnapScreen::generateSprite() {

}

long SnapScreen::to_millis(int minutes, int seconds) {
	return ((minutes * 60) + seconds) * 1000;
}
int SnapScreen::getRandom(int low, int high){
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution<> dist(low, high);
	return dist(engine);
}
