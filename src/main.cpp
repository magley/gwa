#include <assert.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "util/string/string2.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "platform/input.h"
#include "platform/video.h"

#include "entity/entity_test.hpp"
#include "entity/entity.h"


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

string2 from_file(const char* file) {
    std::ifstream ff(file);
    std::stringstream ss;
    ss << ff.rdbuf();
    string2 s = ss.str();
    return s;
}

int main(int argc, char** argv) {
    Entity_Test et;
    et.run();

    int status = SDL_Init(SDL_INIT_VIDEO);
    if (status != 0) {
        handle_sdl_error();
    }

    status = IMG_Init(IMG_INIT_PNG);
    if (status == 0) {
        handle_sdl_error();
    }



    const int view_w = 480;
    const int view_h = 360;
    const int win_w = view_w * 2;
    const int win_h = view_h * 2;

    SDL_Window* win = SDL_CreateWindow("gwa",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       win_w, win_h, SDL_WINDOW_OPENGL);
    if (win == NULL) {
        handle_sdl_error();
    }

    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(win, 
                                                    -1, 
                                                    SDL_RENDERER_ACCELERATED | 
                                                    SDL_RENDERER_PRESENTVSYNC);
    if (sdl_renderer == NULL) {
        handle_sdl_error();
    }
    SDL_RenderSetLogicalSize(sdl_renderer, view_w, view_h);
    SDL_Event event;
    bool is_running = true;

    Input input;
    Texture tex = Texture(sdl_renderer, "../res/img.png");
    Renderer rend = Renderer(sdl_renderer);
    EntityManager em;

    //-------------------------------------------------------------------------
    //
    //

    string2 data = from_file("../res/data1.txt");
    em.load(data);

    EntityID player = 0;
    phys_c* p = em.phys(player);
    body_c* b = em.body(player);

    //
    //
    //-------------------------------------------------------------------------

    while (is_running) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

        input.update();

        if (input.press(SDL_SCANCODE_ESCAPE)) {
            is_running = false;
        }

        em.cleanup();

        //---------------------------------------------------------------------
        //
        //

        p->v.x = (input.down(SDL_SCANCODE_RIGHT) - input.down(SDL_SCANCODE_LEFT)) * (input.down(SDL_SCANCODE_Z) ? 2.5f : 1);
        p->v.y = (input.down(SDL_SCANCODE_DOWN) - input.down(SDL_SCANCODE_UP)) * (input.down(SDL_SCANCODE_Z) ? 2.5f : 1);

        for (EntityID e : em.get_all(CLD | PHYS)) {
            cld_c* cld = em.cld(e);
            cld->build_other(em, e);
        }

        for (EntityID e : em.get_all(PHYS)) {
            phys_c* phys = em.phys(e);
            if ((phys->flags & phys_c::CLD_SOLID) == phys_c::CLD_SOLID) {
                phys->cld_solid(em, e);
            }
        }

        for (EntityID e : em.get_all(PHYS)) {
            phys_c* phys = em.phys(e);
            phys->move(em, e);
        }

        //
        //
        //---------------------------------------------------------------------

        status = rend.clear(96, 96, 96);
        if (status != 0) {
            handle_sdl_error();
        }

        //---------------------------------------------------------------------
        //
        //

        for (EntityID e : em.get_all(0)) {
            if (em.has(e, CLD)) {
                body_c* body = em.body(e);
                cld_c* cld = em.cld(e);

                if (cld->other.size() == 0) {
                    rend.draw_rect(cld->bbox + body->p, {180, 180, 180, 255});
                } else {
                    rend.draw_rect(cld->bbox + body->p, {80, 196, 170, 255});
                }   
            }
        }

        //
        //
        //---------------------------------------------------------------------

        rend.flip();
    }

    SDL_Quit();
    return 0;
}