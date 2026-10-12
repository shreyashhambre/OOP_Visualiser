#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Simulator.h"

class Renderer {
public:
    Renderer(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* smallFont);
    void draw(const Simulator& sim, int width, int height, double animationTime);

    // Returns true if a UI control/concept was clicked.
    bool handleClick(int x, int y, int width, int height, Simulator& sim);
    void handleWheel(int x, int y, int delta, int width, int height);

private:
    SDL_Renderer* r_;
    TTF_Font* font_;
    TTF_Font* smallFont_;
    int codeScroll_ = 0;

    void rect(int x,int y,int w,int h, SDL_Color c, bool fill=true);
    void text(const std::string& s, int x, int y, SDL_Color c, TTF_Font* f=nullptr);
    void centered(const std::string& s, int x, int y, int w, SDL_Color c, TTF_Font* f=nullptr);
    void line(int x1,int y1,int x2,int y2, SDL_Color c);
    void arrow(int x1,int y1,int x2,int y2, SDL_Color c);
    void codePanel(const Simulator& sim, int x,int y,int w,int h);
    void diagram(const Simulator& sim, int x,int y,int w,int h, double animationTime);
    void infoPanel(const Simulator& sim, int x,int y,int w,int h);
    void bottomBar(const Simulator& sim, int x,int y,int w,int h);
    void conceptList(const Simulator& sim, int x,int y,int w,int h);
    void circle(int cx,int cy,int radius,SDL_Color c,bool fill=true);
    void pulseCircle(int cx,int cy,int radius,SDL_Color c,double phase);
    void movingDot(int x1,int y1,int x2,int y2,SDL_Color c,double t);
};
