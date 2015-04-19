#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <ctime>

#include "screen.hpp"
#include "imgui.hpp"
#include "sprite.hpp"
#include "PokemonData.hpp"

class SnapScreen : public Screen {
public:
    SnapScreen();
    ~SnapScreen();
    bool initialize(RenderContext *context, ScreenDispatcher *dispatcher) override;
    void handleEvent(const SDL_Event &sdlEvent) override;
    void frameStep(unsigned long tickMS) override;
    
    void updateSprite(Uint32 dt, bool reverse);
    Uint32 to_millis(int minutes, int seconds);
    std::string to_time(Uint32 millis);
    int getRandom(int low, int high);
    SDL_Texture* textToTexture(std::string, SDL_Color*);
    SDL_Texture* imageToTexture(std::string path);
    Uint32 distanceSquared();
private:
    imgui::UIState m_userInterface;
    
    TTF_Font *font;
    
    SDL_Texture *timerTexture = nullptr;
    std::string timerText;
    SDL_Rect timerRect;
    SDL_Color red = { 255, 0, 0, 0 };
    
    SDL_Texture *scoreTexture = nullptr;
    std::string scoreText;
    SDL_Rect scoreRect;
    SDL_Color blue = { 0, 0, 255, 0 };
    
    SDL_Texture *background;
    
    SDL_Texture *bush;
    SDL_Rect bushes[4];
    int bushWidth = 200;
    int bushHeight = 200;
    
    SDL_Texture *crosshair;
    SDL_Rect crossRect;
    
    Sprite simpleSprite;
    SDL_Rect spriteRect;
    int spriteWidth = 100;
    int spriteHeight = 100;
    
    //Player's score
    int score;
	static const int HIT_POINTS = 10;
    //time of last frame
    Uint32 lastTime;
    //total game duration
    Uint32 countdown = 25 * 1000;//milliseconds
    
    //sprite regen timer
    Uint32 generationTime;
    Uint32 pauseTime;
    
    bool spriteVisible;
    
    //Mouse pointer
    struct point {
        int mx;
        int my;
    };
    point cursor;
    //the current time in-game
    Uint32 gametime = 0;//milliseconds
    
    int phase;
    Uint32 spriteInterval = 3 * 1000;//milliseconds
    Uint32 spriteTravelDist = 200;//pixels
    Uint32 spriteHoldTime = 2 * 1000;//milliseconds
    Uint32 speed = 50;//pixels per second
    struct vec {
        int vx;
        int vy;
    };
    //direction vecs
    vec dir[4];
    int currentDir;
    int currentBush;
};
