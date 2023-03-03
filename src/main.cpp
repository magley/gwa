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

#include "entity/entity.h"
#include "entity/entity_test.hpp"

#include "resource/res_mng.h"

#include "tile/tile.h"

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

void to_file(const char* file, const string2& str) {
    std::ofstream ff(file);
    ff.write(str.c_str(), str.size());
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
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetLogicalSize(sdl_renderer, view_w, view_h);
    SDL_Event event;
    bool is_running = true;

    Input input;
    EntityManager em;
    ResMng res_mng = ResMng(sdl_renderer);
    Renderer rend = Renderer(sdl_renderer, &res_mng);
    TileMap tm;

    tm.sz = vec2(16, 16);
    tm.map = std::vector<std::vector<uint16_t>>(32);
    for (auto& e : tm.map) {
        e = std::vector<uint16_t>(32);
    }

    // TextureH tex_test = res_mng.texture("../res/img.png");

    //-------------------------------------------------------------------------
    //
    //

    string2 data = from_file("../res/data1.txt");
    em.load(data, res_mng);

    string2 data2 = em.save(res_mng);
    to_file("../res/data2.txt", data2);

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

        for (EntityID e : em.get_all(PLAYER)) {
            player_c* player = em.player(e);
            player->move(em, e, input);
        }

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

        for (EntityID e : em.get_all(PLAYER)) {
            player_c* player = em.player(e);
            player->collect_items(em, e);
        }

        for (EntityID e : em.get_all(ITEM)) {
            item_c* item = em.item(e);
            item->update(em, e);
        }


        //
        //
        //---------------------------------------------------------------------

        status = rend.clear(64, 64, 64);
        if (status != 0) {
            handle_sdl_error();
        }

        //---------------------------------------------------------------------
        //
        //

        for (fp6 yy = 0; yy < win_h; yy += tm.sz.y) {
            rend.line(vec2(0, yy), vec2(win_w, yy), {64, 0, 64, 255});
        }

        for (fp6 xx = 0; xx < win_w; xx += tm.sz.x) {
            rend.line(vec2(xx, 0), vec2(xx, win_h), {64, 0, 64, 255});
        }

        for (EntityID e : em.get_all(0)) {
            if (em.has(e, CLD | PLAYER)) {
                const BBox bbox = em.cld(e)->bbox + em.body(e)->p;
                auto tiles_touching = tm.touching(bbox);

                for (auto& t : tiles_touching) {
                    BBox rect = BBox::from(vec2(t.x * tm.sz.x, t.y * tm.sz.y), tm.sz);
                    rend.rectf(rect, {96, 0, 96, 255});
                }
            }

            if (em.has(e, CLD)) {
                body_c* body = em.body(e);
                cld_c* cld = em.cld(e);

                rend.rect(cld->bbox + body->p, {180, 180, 180, 255});
                if (cld->other.size() == 0) {
                } else {
                    rend.rectf(cld->bbox + body->p, {32, 255, 96, 96});
                }
            }

            if (em.has(e, SPR)) {
                body_c* body = em.body(e);
                spr_c* spr = em.spr(e);
                TextureH tex = spr->tex;

                if (em.has(e, CLD)) {
                    cld_c* cld = em.cld(e);
                    rend.tex_sized(tex, body->p, 0, cld->bbox.size());
                } else {
                    rend.tex(tex, body->p, 0);
                }
            }
        }

        //
        //
        //---------------------------------------------------------------------

        rend.swap_buffers();
    }

    SDL_Quit();
    return 0;
}