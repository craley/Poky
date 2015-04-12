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
    
    void generateSprite();
    long to_millis(int minutes, int seconds);
	std::string to_time(long millis);
    int getRandom(int low, int high);
    SDL_Texture* textToTexture(std::string, SDL_Color);
    SDL_Texture* imageToTexture(std::string path);
    
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
    
    Sprite simpleSprite;

    int score;
    std::clock_t time;
    long countdown;
    
    bool spriteVisible;
    
    //Mouse pointer
    //int cursor_x;
    //int cursor_y;
    struct point {
        int mx;
        int my;
    };
    point cursor;
};
