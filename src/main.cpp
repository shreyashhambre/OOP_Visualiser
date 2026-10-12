#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <chrono>
#include "Simulator.h"
#include "Renderer.h"

static TTF_Font* loadFont(int size) {
    const char* paths[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
    };
    for (auto p : paths) {
        TTF_Font* f = TTF_OpenFont(p,size);
        if(f) return f;
    }
    return nullptr;
}

int main() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }
    if(TTF_Init() != 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Window* window=SDL_CreateWindow(
        "OOP Visualizer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 760,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if(!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        TTF_Quit(); SDL_Quit(); return 1;
    }

    SDL_Renderer* renderer=SDL_CreateRenderer(
        window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window); TTF_Quit(); SDL_Quit(); return 1;
    }

    TTF_Font* font=loadFont(18);
    TTF_Font* small=loadFont(14);
    if(!font || !small) {
        std::cerr << "Could not load a system font.\n";
        if(font) TTF_CloseFont(font);
        if(small) TTF_CloseFont(small);
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
        TTF_Quit(); SDL_Quit(); return 1;
    }

    Simulator sim;
    Renderer ui(renderer,font,small);

    bool quit=false;
    auto last=std::chrono::steady_clock::now();
    double accumulator=0.0;

    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type==SDL_QUIT) quit=true;
            else if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_ESCAPE) quit=true;
            else if(e.type==SDL_MOUSEWHEEL) {
                int mx,my;
                SDL_GetMouseState(&mx,&my);
                int w,h;
                SDL_GetWindowSize(window,&w,&h);
                ui.handleWheel(mx,my,e.wheel.y,w,h);
            }
            else if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                int w,h;
                SDL_GetWindowSize(window,&w,&h);
                ui.handleClick(e.button.x,e.button.y,w,h,sim);
            }
        }

        auto now=std::chrono::steady_clock::now();
        std::chrono::duration<double> dt=now-last;
        last=now;

        if(sim.isRunning()) {
            accumulator += dt.count();
            if(accumulator >= 0.75) {
                sim.step();
                accumulator=0.0;
            }
        } else {
            accumulator=0.0;
        }

        int w,h;
        SDL_GetWindowSize(window,&w,&h);
        ui.draw(sim,w,h,std::chrono::duration<double>(now.time_since_epoch()).count());
    }

    TTF_CloseFont(font);
    TTF_CloseFont(small);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
