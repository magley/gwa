#include <assert.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <set>
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
#include "platform/font.h"
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

//=============================================================================
// REND UTILITY
//=============================================================================

struct RendAgent {
    union {
        EntityID entity;
        int tile_layer;
    };
    enum { ENTITY, TILE_LAYER };
    int type;
    int8_t depth;

    static RendAgent from_entity(EntityID e, GwaCtx& ctx) {
        RendAgent a;
        a.entity = e;
        a.type = ENTITY;
        a.depth = ctx.em->body(e)->depth;
        return a;
    }

    static RendAgent from_tilemap(int tilemap_layer, GwaCtx& ctx) {
        RendAgent a;
        a.tile_layer = tilemap_layer;
        a.type = TILE_LAYER;
        a.depth = ctx.tm->layers[a.tile_layer].depth;
        return a;
    }
};

int operator<(const RendAgent& a, const RendAgent& b) {
    if (a.depth < b.depth) return -1;
    if (a.depth > b.depth) return 1;
    return 0;
}

//=============================================================================
// main()
//=============================================================================

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

    GwaCtx ctx(&input, &rend, &em, &res_mng, &tm, vec2(view_w, view_h));
    res_mng.init_ctx(&ctx);
    fp6 tile_anim_frame = 0;
    fp6 tile_anim_spd = 0.15;

    FontH font_small = res_mng.font("../res/font_small.font");

    //=========================================================================

    em.load(ctx, from_file("../res/data1.txt"));
    //tm.load(ctx, from_file("../res/lvl_test.tile"));

    // TOOD: Game will crash if
    // 1) 0 tilemap layers (OOB)
    // 2) 0th tilemap layer's map.size() or map[0].size() are 0 (OOB or divby0)
    // 3) oth tilemap layer's sz is (0, 0) (divby0)
    // So, please make it that this doesn't happen. Or the level editor should
    // always have one minimal TileMapLayer (see below).

    TileMapLayer tml = TileMapLayer();
    tml.tileset = res_mng.tileset("../res/test.tileset");
    tml.map.push_back({-1});
    tml.depth = 10;
    tml.sz = vec2(16, 16);
    tm.layers.push_back(tml);



    std::multiset<RendAgent> rend_agents; // Because of depth.

    TextureH bg_sky_gradient = res_mng.texture("../res/bg_skygrad.png");

    const vec2 cam_max = vec2(
        view_w * 2,
        view_h * 1
    );
    

    bool editor_rend_grid = true;
    int editor_focused_tile_layer = 0;
    bool editor_dim_unfocused_layers = false;

    int editor_tile_index = 0;

    while (is_running) {
        input.update_wheel(0);
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            } else if (event.type == SDL_MOUSEWHEEL) {
                input.update_wheel(-event.wheel.y);
            }
        }

        input.update();

        if (input.press(SDL_SCANCODE_F1)) {
            editor_rend_grid ^= true;
        }
        if (input.press(SDL_SCANCODE_F2)) {
            editor_dim_unfocused_layers ^= true;
        }
        editor_focused_tile_layer += input.press(SDL_SCANCODE_EQUALS) - input.press(SDL_SCANCODE_MINUS);
        if (editor_focused_tile_layer < 0) {
            editor_focused_tile_layer += ctx.tm->layers.size();
        }
        editor_focused_tile_layer %= ctx.tm->layers.size();

        editor_tile_index += input.m_w() > 0 ? 1 : input.m_w() < 0 ? -1 : 0;
        if (editor_tile_index < 0) {
            editor_tile_index += ctx.rm->tileset(ctx.tm->layers[editor_focused_tile_layer].tileset)->tiles.size();
        }
        editor_tile_index %= ctx.rm->tileset(ctx.tm->layers[editor_focused_tile_layer].tileset)->tiles.size();

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
            ctx.cam = body->p - ctx.view_sz / 2;
            ctx.cam.x = ctx.cam.x.clamp(0, cam_max.x - ctx.view_sz.x);
            ctx.cam.y = ctx.cam.y.clamp(0, cam_max.y - ctx.view_sz.y);
            break;
        }

        tile_anim_frame += tile_anim_spd;

        ///// UPDATE EDITOR
        {
            if (input.m_down(SDL_BUTTON_LMASK)) {
                TileMapLayer& layer = ctx.tm->layers[editor_focused_tile_layer];
                Tileset* tileset = ctx.rm->tileset(layer.tileset);
                int x = (input.m_x() / 3 + ctx.cam.x) / layer.sz.x;
                int y = (input.m_y() / 3 + ctx.cam.y) / layer.sz.y;
                const Tile t = tileset->tiles[editor_tile_index];

                for (int i = layer.map.size(); i <= y; i++) {
                    layer.map.push_back({});
                }
                for (int i = 0; i < layer.map.size(); i++) {
                    for (int j = layer.map[i].size(); j <= x; j++) {
                        layer.map[i].push_back(-1);
                    }
                }

                layer.map[y][x] = editor_tile_index;
            }
        }
        ////////////////////////////////////////

        //=====================================================================

        status = rend.clear(128, 128, 128);
        if (status != 0) {
            handle_sdl_error();
        }

        rend.tex(bg_sky_gradient, vec2(0, 0), 0);

        /////// RENDER EDITOR BEFORE
        {
            TileMapLayer& layer = ctx.tm->layers[editor_focused_tile_layer];

            if (editor_rend_grid) {
                for (fp6 yy = 0; yy <= view_h + layer.sz.y; yy += layer.sz.y) {
                    const fp6 y_ = (int)((yy + ctx.cam.y) / layer.sz.y) * layer.sz.y;
                    rend.line(vec2(ctx.cam_extents().l, y_) - ctx.cam, vec2(ctx.cam_extents().r, y_) - ctx.cam, {255, 255, 255, 64});
                }

                for (fp6 xx = 0; xx <= view_w + layer.sz.x; xx += layer.sz.x) {
                    const fp6 x_ = (int)((xx + ctx.cam.x) / layer.sz.x) * layer.sz.x;
                    rend.line(vec2(x_, ctx.cam_extents().u) - ctx.cam, vec2(x_, ctx.cam_extents().d) - ctx.cam, {255, 255, 255, 64});
                }
            }
        }
        ///////////////////////////////////


        rend_agents.clear();
        for (int i = 0; i < tm.layers.size(); i++) {
            rend_agents.insert(RendAgent::from_tilemap(i, ctx));
        }
        for (EntityID e : em.get_all(0)) {
            RendAgent ag = RendAgent::from_entity(e, ctx);
            int x = rend_agents.find(ag) == rend_agents.end();
            rend_agents.insert(ag);
        }

        for (const RendAgent& ra : rend_agents) {
            switch (ra.type) {
                case RendAgent::ENTITY: {
                    ctx.em->rend(ra.entity, ctx);
                } break;
                case RendAgent::TILE_LAYER: {
                    const bool dim_layer = 
                            editor_dim_unfocused_layers && 
                            (editor_focused_tile_layer != ra.tile_layer);
                    const TileMapLayer& layer = ctx.tm->layers[ra.tile_layer];
                    layer.rend(ctx, tile_anim_frame, dim_layer);
                } break;
                default: {

                } break;
            }
        }

        /////// RENDER EDITOR AFTER
        {
            TileMapLayer& layer = ctx.tm->layers[editor_focused_tile_layer];
            Tileset* tileset = ctx.rm->tileset(layer.tileset);
            const Tile t = tileset->tiles[editor_tile_index];
            const int x = (input.m_x() / 3 + ctx.cam.x) / layer.sz.x;
            const int y = (input.m_y() / 3 + ctx.cam.y) / layer.sz.y;

            t.rend(ctx, tileset, vec2(x, y), layer.sz, tile_anim_frame);
        }
        ///////////////////////////////////

        rend.text(vec2(32, 32), font_small, "Hello\n1234 ~3820^{}[]");

        rend.rect(BBox::from(vec2(0, 0) - ctx.cam, cam_max), {0, 255, 0, 255});
        rend.swap_buffers();
    }

    SDL_Quit();
    return 0;
}