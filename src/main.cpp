#include <assert.h>
#include <stdio.h>
#include <vector>

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

    EntityID e1 = em.create();
    em.add(e1, CLD | PHYS);
    body_c* b = em.body(e1);
    cld_c* c = em.cld(e1);
    phys_c* p = em.phys(e1);
    b->p = vec2(128, 32);
    p->flags |= phys_c::CLD_SOLID;
    c->bbox = BBox::from(vec2(), vec2(16, 32));

    EntityID e2 = em.create();
    em.add(e2, CLD | PHYS);
    body_c* b2 = em.body(e2);
    cld_c* c2 = em.cld(e2);
    phys_c* p2 = em.phys(e2);
    b2->p = vec2(128, 128);
    c2->flags = cld_c::SOLID;
    c2->bbox = BBox::from(vec2(), vec2(64, 64));

    EntityID e3 = em.create();
    em.add(e3, CLD | PHYS);
    body_c* b3 = em.body(e3);
    cld_c* c3 = em.cld(e3);
    phys_c* p3 = em.phys(e3);
    b3->p = vec2(192, 128);
    c3->flags = cld_c::SOLID;
    c3->bbox = BBox::from(vec2(), vec2(64, 64));

    fp6 move_em = 0;

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

        if (input.press(SDL_SCANCODE_SPACE)) {
            move_em += 1;
            if (move_em > 1) {
                move_em = -1;
            }
        }

        p2->v.y = -0.2 * move_em;
        p3->v.y = -0.25 * move_em;
        p2->v.x = -0.3 * move_em;
        p3->v.x = -0.127 * move_em;
        
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