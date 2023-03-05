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
    int status = SDL_Init(SDL_INIT_VIDEO);
    if (status != 0) {
        handle_sdl_error();
    }

    status = IMG_Init(IMG_INIT_PNG);
    if (status == 0) {
        handle_sdl_error();
    }

    const int view_w = 480;
    const int view_h = 270;
    const int win_w = view_w * 3;
    const int win_h = view_h * 3;

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
    tm.tileset = { Tile(0), Tile(cld_c::SOLID), Tile(cld_c::SOLID_F), Tile(cld_c::SOLID_L) };
    tm.map = std::vector<std::vector<uint16_t>>(17);
    for (auto& e : tm.map) {
        e = std::vector<uint16_t>(30);
    }

    int k = 0;
    for (int y = 0; y < tm.map.size(); y += 3) {
        for (int x = 0; x < tm.map[y].size(); x += 3) {
            tm.map[y][x] = (k++) % 4;
        }
    }

    TextureH txt_tst_test = res_mng.texture("../res/tst_test.png");

    //-------------------------------------------------------------------------
    //
    //

    string2 data = from_file("../res/data1.txt");
    em.load(data, res_mng);

    string2 data2 = em.save(res_mng);
    to_file("../res/data2.txt", data2);

    string2 tile_data2 = tm.save();
    to_file("../res/lvl_test.tile", tile_data2);

    string2 tile_data = from_file("../res/lvl_test.tile");
    tm.load(tile_data);

    string2 tile_data3 = tm.save();
    to_file("../res/lvl_test2.tile", tile_data3);

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
            cld->build_tilemap_range(em, e, tm);
        }

        for (EntityID e : em.get_all(PHYS)) {
            phys_c* phys = em.phys(e);
            if ((phys->flags & phys_c::CLD_SOLID) == phys_c::CLD_SOLID) {
                phys->cld_solid(em, e, tm);
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

        status = rend.clear(128, 128, 128);
        if (status != 0) {
            handle_sdl_error();
        }

        //---------------------------------------------------------------------
        //
        //

        // for (fp6 yy = 0; yy < win_h; yy += tm.sz.y) {
        //     rend.line(vec2(0, yy), vec2(win_w, yy), {96, 96, 96, 255});
        // }

        // for (fp6 xx = 0; xx < win_w; xx += tm.sz.x) {
        //     rend.line(vec2(xx, 0), vec2(xx, win_h), {96, 96, 96, 255});
        // }


        ///
        const Texture* txt_tst = res_mng.texture(txt_tst_test);
        const int w = txt_tst->w;
        const int h = txt_tst->h;

        for (uint16_t y = 0; y < tm.map.size(); y++) {
            if (y * tm.sz.y < 0 || (y + 1) * tm.sz.y >= view_h) {
                break;
            }
            for (uint16_t x = 0; x < tm.map[y].size(); x++) {
                if (x * tm.sz.x < 0 || (x + 1) * tm.sz.x >= view_w) {
                    break;
                }
                const uint16_t tindex = tm.map[y][x];
                const Tile t = tm.tileset[tindex];
                const uint8_t v = t.v;



                const fp6 xx = (tindex % w) * tm.sz.x;
                const fp6 yy = (tindex / w) * tm.sz.y;
                const BBox src = BBox::from(vec2(xx, yy), tm.sz);
                rend.tex(txt_tst_test, vec2(x * tm.sz.x, y * tm.sz.y), 0, src);

                // if ((v & cld_c::SOLID_F) == cld_c::SOLID_F) {
                //     rend.rectf(BBox::from(vec2(x * tm.sz.x, y * tm.sz.y), vec2(tm.sz.x, 2)), {255, 0, 0, 255});
                // }
                // if ((v & cld_c::SOLID_C) == cld_c::SOLID_C) {
                //     rend.rectf(BBox::from(vec2(x * tm.sz.x, y * tm.sz.y + tm.sz.y - 2), vec2(tm.sz.x, 2)), {255, 0, 0, 255});
                // }
                // if ((v & cld_c::SOLID_L) == cld_c::SOLID_L) {
                //     rend.rectf(BBox::from(vec2(x * tm.sz.x + tm.sz.x - 2, y * tm.sz.y), vec2(2, tm.sz.y)), {255, 0, 0, 255});
                // }
                // if ((v & cld_c::SOLID_R) == cld_c::SOLID_R) {
                //     rend.rectf(BBox::from(vec2(x * tm.sz.x, y * tm.sz.y), vec2(2, tm.sz.y)), {255, 0, 0, 255});
                // }
            }
        }

        for (EntityID e : em.get_all(0)) {
            if (em.has(e, CLD | PLAYER)) {
                const BBox bbox = em.cld(e)->bbox + em.body(e)->p;
                auto tiles_touching = tm.touching(bbox);

                for (auto& t : tiles_touching) {
                    BBox rect = BBox::from(vec2(t.x * tm.sz.x, t.y * tm.sz.y), tm.sz);
                    rend.rectf(rect, {64, 64, 64, 96});
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