
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
	//Timer
	time = std::clock();
	countdown = to_millis(3, 0);
	//mouse cursor - centered
	cursor.mx = WINDOW_WIDTH * 0.5;
	cursor.my = WINDOW_HEIGHT * 0.5;
	//timer display
	font = TTF_OpenFont("assets/unifont-7.0.06.ttf", 30);
	
	timerText = std::to_string(countdown);
	timerTexture = textToTexture(timerText, red);
	timerRect = { WINDOW_WIDTH - 150 - 10, 5, 150, 50 };
	
	scoreText = "0";
	scoreTexture = textToTexture(scoreText, blue);
	scoreRect = { 20, 5, 40, 40 };

	simpleSprite.setImage(context->loadTexture("assets/sprites/Spr_5b_025.png"));
	simpleSprite.setScale(100,100);
	

	background = imageToTexture("assets/pokyBackground.jpg");
	spriteVisible = false;

	return true;
}
SDL_Texture* SnapScreen::textToTexture(std::string text, SDL_Color color){
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* message = SDL_CreateTextureFromSurface(m_context->renderer, surface);
	SDL_FreeSurface(surface);
	return message;
}
SDL_Texture* SnapScreen::imageToTexture(std::string path){
	SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(m_context->renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
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
	

	//rendering::

	//clear target
	SDL_RenderClear(m_context->renderer);
	//draw background
	SDL_RenderCopy(m_context->renderer, background, nullptr, nullptr);
	
	//draw timer
	timerText = to_time(countdown);
	timerTexture = textToTexture(timerText, red);
	SDL_RenderCopy(m_context->renderer, timerTexture, nullptr, &timerRect);
	
	//draw score
	scoreText = std::to_string(score);
	timerTexture = textToTexture(scoreText, blue);
	SDL_RenderCopy(m_context->renderer, scoreTexture, nullptr, &scoreRect);
	
	if(spriteVisible){
		m_context->render(simpleSprite);
	}

	//copy portion of texture to render target
	//SDL_RenderCopy(m_context->renderer, timerTexture, nullptr, &timerRect);
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
std::string SnapScreen::to_time(long millis){
		long totalSeconds = millis / 1000;
		int minutes = totalSeconds / 60;
		int seconds = totalSeconds % 60;
		return std::to_string(minutes) + " : " + std::to_string(seconds);
}
