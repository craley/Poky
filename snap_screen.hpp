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
    int getRandom(int low, int high);
    
private:
    imgui::UIState m_userInterface;
    
    //timer text box
    TTF_Font *font;
    SDL_Texture *timerTexture = nullptr;
    std::string timerText;
    SDL_Rect timerRect;
    
    //sprites
    Sprite background;
    Sprite simpleSrite;

    int score;
    std::clock_t time;
    long countdown;
    
    //Mouse pointer
    //int cursor_x;
    //int cursor_y;
    struct point {
        int mx;
        int my;
    };
    point cursor;
};