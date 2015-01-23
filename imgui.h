#pragma once

namespace ui {
	class Context {
    public:
        int mouseX = 0;
        int mouseY = 0;
        int hotItem = -1;
        int activeItem = -1;
        bool mouseDown = false;

        void begin();
        void end();

        bool mouseHit(int x, int y, int w, int h);
        void drawRect(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color);
        bool button(int id, SDL_Renderer *renderer, int x, int y, int w, int h);
	};
} // namespace ui
