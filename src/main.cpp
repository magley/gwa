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

#include "ctx/ctx.h"
#include "util/file/futil.h"

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

    //=========================================================================

    Input input;
    EntityManager em;
    ResMng res_mng = ResMng(sdl_renderer);
    Renderer rend = Renderer(sdl_renderer, &res_mng);
    TileMap tm;

    GwaCtx ctx(&input, &rend, &em, &res_mng, &tm);
    res_mng.init_ctx(&ctx);
    fp6 tile_anim_frame = 0;
    fp6 tile_anim_spd = 0.15;

    //=========================================================================

    em.load(ctx, from_file("../res/data1.txt"));
    to_file("../res/data2.txt", em.save(ctx));
    tm.load(ctx, from_file("../res/lvl_test.tile"));
    to_file("../res/lvl_test2.tile", tm.save(ctx));

    // to_file("../res/test2.tileset", res_mng.tileset(tm.tileset)->save(ctx));

    TextureH bg_sky_gradient = res_mng.texture("../res/bg_skygrad.png");

    vec2 cam;
    const vec2 cam_max = vec2(
        (int)tm.layers[0].map[0].size() * tm.layers[0].sz.x, 
        (int)tm.layers[0].map.size() * tm.layers[0].sz.y
    );
    const vec2 view_sz = vec2(view_w, view_h);

    bool rend_grid = true;

    while (is_running) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

        input.update();

        if (input.press(SDL_SCANCODE_F1)) {
            rend_grid ^= true;
        }

        if (input.press(SDL_SCANCODE_ESCAPE)) {
            is_running = false;
        }

        em.cleanup();

        //=====================================================================

        for (EntityID e : em.get_all(PLAYER)) {
            player_c* player = em.player(e);
            player->move(ctx, e);
        }

        for (EntityID e : em.get_all(CLD | PHYS)) {
            cld_c* cld = em.cld(e);
            cld->build_other(ctx, e);
            cld->build_tilemap_range(ctx, e);
        }

        for (EntityID e : em.get_all(PHYS)) {
            phys_c* phys = em.phys(e);
            if ((phys->flags & phys_c::CLD_SOLID) == phys_c::CLD_SOLID) {
                phys->cld_solid(ctx, e);
            }
        }

        for (EntityID e : em.get_all(PHYS)) {
            phys_c* phys = em.phys(e);
            phys->move(ctx, e);
        }

        for (EntityID e : em.get_all(PLAYER)) {
            player_c* player = em.player(e);
            player->collect_items(ctx, e);
        }

        for (EntityID e : em.get_all(ITEM)) {
            item_c* item = em.item(e);
            item->update(ctx, e);
        }

        for (EntityID e : em.get_all(PLAYER)) {
            body_c* body = em.body(e);
            cam = body->p - view_sz / 2;
            cam.x = cam.x.clamp(0, cam_max.x - view_sz.x);
            cam.y = cam.y.clamp(0, cam_max.y - view_sz.y);
            break;
        }

        tile_anim_frame += tile_anim_spd;

        //=====================================================================

        status = rend.clear(128, 128, 128);
        if (status != 0) {
            handle_sdl_error();
        }

        //=====================================================================

        const BBox cam_extents = BBox::from(cam, view_sz);

        rend.tex(bg_sky_gradient, vec2(0, 0), 0);

        // TODO: Show grid when editing a layer (only that specific layer).
        // if (rend_grid) {
        //     for (fp6 yy = 0; yy <= view_h + tm.sz.y; yy += tm.sz.y) {
        //         const fp6 y_ = (int)((yy + cam.y) / tm.sz.y) * tm.sz.y;
        //         rend.line(vec2(cam_extents.l, y_) - cam, vec2(cam_extents.r, y_) - cam, {255, 255, 255, 64});
        //     }

        //     for (fp6 xx = 0; xx <= view_w + tm.sz.x; xx += tm.sz.x) {
        //         const fp6 x_ = (int)((xx + cam.x) / tm.sz.x) * tm.sz.x;
        //         rend.line(vec2(x_, cam_extents.u) - cam, vec2(x_, cam_extents.d) - cam, {255, 255, 255, 64});
        //     }
        // }

        for (const TileMapLayer& layer : tm.layers) {
            Tileset* tileset = res_mng.tileset(layer.tileset);
            const Texture* txt_tst = res_mng.texture(tileset->tex);
            const vec2 sz = tileset->sz;

            for (uint16_t y = 0; y < layer.map.size(); y++) {
                if ((y + layer.sz.y) * layer.sz.y < cam_extents.u) continue;
                if (y * layer.sz.y > cam_extents.d) break;
                for (uint16_t x = 0; x < layer.map[y].size(); x++) {
                    if ((x + layer.sz.x) * layer.sz.x < cam_extents.l) continue;
                    if (x * layer.sz.x > cam_extents.r) break;

                    const uint16_t tindex = layer.map[y][x];
                    const Tile t = tileset->tiles[tindex];
                    const uint8_t v = t.v;

                    int tile_anim_index = (int)(tile_anim_frame % (int)t.pos.size());

                    const fp6 xx = t.pos[tile_anim_index].x;
                    const fp6 yy = t.pos[tile_anim_index].y;
                    const BBox src = BBox::from(vec2(xx, yy), sz);
                    rend.tex(tileset->tex, vec2(x * layer.sz.x, y * layer.sz.y) - cam, 0, src);
                }
            }
        }

        for (EntityID e : em.get_all(0)) {
            if (em.has(e, CLD)) {
                body_c* body = em.body(e);
                cld_c* cld = em.cld(e);

                rend.rect(cld->bbox + (body->p - cam), {180, 180, 180, 255});
                if (cld->other.size() == 0) {
                } else {
                    rend.rectf(cld->bbox + (body->p - cam), {32, 255, 96, 96});
                }
            }

            if (em.has(e, SPR)) {
                body_c* body = em.body(e);
                spr_c* spr = em.spr(e);
                TextureH tex = spr->tex;

                if (em.has(e, CLD)) {
                    cld_c* cld = em.cld(e);
                    rend.tex_sized(tex, body->p - cam, 0, cld->bbox.size());
                } else {
                    rend.tex(tex, body->p - cam, 0);
                }
            }
        }

        rend.rect(BBox::from(vec2(0, 0) - cam, cam_max), {0, 255, 0, 255});

        //=====================================================================

        rend.swap_buffers();
    }

    SDL_Quit();
    return 0;
}