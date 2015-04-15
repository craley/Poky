
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
	
	//mouse cursor - centered
	cursor.mx = WINDOW_WIDTH * 0.5;
	cursor.my = WINDOW_HEIGHT * 0.5;
	//timer display
	font = TTF_OpenFont("assets/unifont-7.0.06.ttf", 32);

	//timerText = std::to_string(countdown);
	timerText = to_time(countdown);
	timerTexture = textToTexture(timerText, &red);
	timerRect = {WINDOW_WIDTH - 150 - 10, 5, 150, 50}; //x,y,w,h

	scoreText = "0";
	scoreTexture = textToTexture(scoreText, &blue);
	scoreRect = {20, 5, 40, 40};

	simpleSprite.setImage(context->loadTexture("assets/sprites/Spr_5b_025.png"));
	simpleSprite.setPosition(0,0);
	simpleSprite.setScale(spriteWidth, spriteHeight);
	spriteRect = simpleSprite.rect();

	background = imageToTexture("assets/pokyBackground.jpg");
	spriteVisible = false;

	crosshair = imageToTexture("assets/crosshair.png");
	crossRect = {(int) (WINDOW_WIDTH * 0.5f) - 20, (int) (WINDOW_HEIGHT * 0.5f) - 20, 40, 40};

	bush = imageToTexture("assets/bush.png");
	bushes[0] = {(int) (WINDOW_WIDTH * 0.1), (int) (WINDOW_HEIGHT * 0.33),  bushWidth, bushHeight };
	bushes[1] = {(int) (WINDOW_WIDTH * 0.25), (int) (WINDOW_HEIGHT * 0.74), bushWidth, bushHeight };
	bushes[2] = {(int) (WINDOW_WIDTH * 0.55), (int) (WINDOW_HEIGHT * 0.25), bushWidth, bushHeight };
	bushes[3] = {(int) (WINDOW_WIDTH * 0.65), (int) (WINDOW_HEIGHT * 0.70), bushWidth, bushHeight };
	
	
	dir[0] = {  1,  0 };
	dir[1] = { -1,  0 };
	dir[2] = {  0,  1 };
	dir[3] = {  0, -1 };
	
	return true;
}

SDL_Texture* SnapScreen::textToTexture(std::string text, SDL_Color *color) {
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), *color);
	SDL_Texture* message = SDL_CreateTextureFromSurface(m_context->renderer, surface);
	SDL_FreeSurface(surface);
	return message;
}

SDL_Texture* SnapScreen::imageToTexture(std::string path) {
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (!surface) {
		std::cerr << "Image error: " << IMG_GetError() << std::endl;
		return nullptr;
	}
	SDL_Texture *tex = SDL_CreateTextureFromSurface(m_context->renderer, surface);
	SDL_FreeSurface(surface);
	return tex;
}

void SnapScreen::handleEvent(const SDL_Event &sdlEvent) {
	switch (sdlEvent.type) {
		case SDL_MOUSEBUTTONDOWN:
			return;
		case SDL_MOUSEMOTION:
		default:
			return;
	}
}

void SnapScreen::frameStep(unsigned long now) {
	if(lastTime == 0){
		lastTime = now;//how to init on every entry??
	}
	Uint32 elapsed = now - lastTime;
	gametime += elapsed;
	if (gametime > countdown) {
		//game over
	}
	lastTime = now;
	if (spriteVisible) {
		switch (phase) {
			case 0:
				updateSprite(elapsed, false);
				if(distanceSquared() > (spriteTravelDist * spriteTravelDist)){
					phase = 1;
					pauseTime = 0;
				}
				break;
			case 1:
				pauseTime += elapsed;
				if(pauseTime > spriteHoldTime){
					phase = 2;
				}
				break;
			case 2:
				updateSprite(elapsed, true);
				if(distanceSquared() < 20){
					spriteVisible = false;
				}
		}
	} else {
		generationTime += elapsed;
		if(generationTime > spriteInterval){
			generationTime = 0;
			phase = 0;
			spriteVisible = true;
			currentBush = getRandom(0, 4);
			currentDir = getRandom(0, 4);
			spriteRect.x = bushes[currentBush].x + ((int)(bushes[currentBush].w * 0.5f) - (int)(spriteRect.w * 0.5f));
			spriteRect.y = bushes[currentBush].y + ((int)(bushes[currentBush].h * 0.5f) - (int)(spriteRect.h * 0.5f));
		}
	}

	//rendering::

	//clear target
	SDL_RenderClear(m_context->renderer);
	//draw background
	SDL_RenderCopy(m_context->renderer, background, nullptr, nullptr); //stretch
	
	if (spriteVisible) {
		m_context->render(simpleSprite);
	}

	//draw bushes
	SDL_RenderCopy(m_context->renderer, bush, nullptr, &(bushes[0]));
	SDL_RenderCopy(m_context->renderer, bush, nullptr, &(bushes[1]));
	SDL_RenderCopy(m_context->renderer, bush, nullptr, &(bushes[2]));
	SDL_RenderCopy(m_context->renderer, bush, nullptr, &(bushes[3]));

	//draw timer
	timerText = to_time(countdown - gametime);
	timerTexture = textToTexture(timerText, &red);
	SDL_RenderCopy(m_context->renderer, timerTexture, nullptr, &timerRect);

	//draw score
	scoreText = std::to_string(score);
	timerTexture = textToTexture(scoreText, &blue);
	SDL_RenderCopy(m_context->renderer, scoreTexture, nullptr, &scoreRect);

	
	//draw crosshair
	SDL_RenderCopy(m_context->renderer, crosshair, nullptr, &crossRect);

	//copy portion of texture to render target
	//SDL_RenderCopy(m_context->renderer, timerTexture, nullptr, &timerRect);
	//m_userInterface.end();

	//update screen with any rendering performed since last update
	SDL_RenderPresent(m_context->renderer);
}

//update spriteRect
void SnapScreen::updateSprite(Uint32 dt, bool reverse) {
	int move = dt * (speed / 1000);
	spriteRect.x += (move * dir[currentDir].vx * (reverse ? -1 : 1));
	spriteRect.y += (move * dir[currentDir].vy * (reverse ? -1 : 1));
}

Uint32 SnapScreen::to_millis(int minutes, int seconds) {
	return ((minutes * 60) + seconds) * 1000;
}

int SnapScreen::getRandom(int low, int high) {
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution<> dist(low, high);
	return dist(engine);
}

Uint32 SnapScreen::distanceSquared() {
	SDL_Rect& a = spriteRect;
	SDL_Rect& b = bushes[currentBush];
	return ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.x));
}

std::string SnapScreen::to_time(Uint32 millis) {
	Uint32 totalSeconds = millis / 1000;
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;
	return std::to_string(minutes) + " : " + std::to_string(seconds);
}
