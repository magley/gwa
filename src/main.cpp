#include <stdio.h>
#include "SDL2/SDL.h"

void print_err(const char* prefix, const char* msg, const char* suffix) {
    printf("\033[0;37m%s\033[0;31m%s\033[0;37m%s", prefix, msg, suffix);
}

void print_sdl_error() {
    const char* error = SDL_GetError();
    print_err("SDL2: ", error, "\n");
}

void handle_sdl_error() {
    print_sdl_error();
    exit(1);
}

int main(int argc, char** argv) {
    int status = SDL_Init(SDL_INIT_VIDEO);
    if (status != 0) {
        handle_sdl_error();
    }

    SDL_Window* win = SDL_CreateWindow("gwa", 
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED, 
                                        640, 480, SDL_WINDOW_OPENGL);
    if (win == NULL) {
        handle_sdl_error();
    }

    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (rend == NULL) {
        handle_sdl_error();
    }

    SDL_Event event;
    bool is_running = true;

    while (is_running) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

        status = SDL_SetRenderDrawColor(rend, 128, 128, 128, 255);
        if (status != 0) {
            handle_sdl_error();
        }

        status = SDL_RenderClear(rend);
        if (status != 0) {
            handle_sdl_error();
        }

        SDL_RenderPresent(rend);
    }

    SDL_Quit();
    return 0;
}