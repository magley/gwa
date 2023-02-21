#include <stdio.h>
#include "SDL2/SDL.h"

void print_err(const char* prefix, const char* msg, const char* suffix) {
    printf("\033[0;37m%s\033[0;31m%s\033[0;37m%s", prefix, msg, suffix);
}

void handle_sdl_error() {
    const char* error = SDL_GetError();
    print_err("SDL2: ", error, "\n");
}

int main(int argc, char** argv) {
    int status = SDL_Init(SDL_INIT_VIDEO);
    if (status != 0) {
        handle_sdl_error();
        return 1;
    }

    SDL_Quit();
    return 0;
}