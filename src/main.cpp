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
#include "editor/editor.h"

#include "gui/gui.h"

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

#include "platform/rendagent.h"

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
    const int win_h = view_h * 3;// TODO: Sync up 3 with GwaCtx::scale.

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
    Editor editor;
    Gui gui;

    GwaCtx ctx(&input, &rend, &em, &res_mng, &tm, &gui, vec2(view_w, view_h));
    input.init_ctx(&ctx);
    res_mng.init_ctx(&ctx);
    gui.init_ctx(&ctx);

    fp6 tile_anim_spd = 0.15;

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
        gui.begin();

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

        ctx.tile_anim += tile_anim_spd;

        editor.on_input(ctx);

        //=====================================================================

        rend.clear(128, 128, 128);
        rend.tex(bg_sky_gradient, vec2(0, 0), 0);

        editor.render_before(ctx);

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
                            editor.tile_focuslayer && 
                            (editor.tile_layer != ra.tile_layer);
                    const TileMapLayer& layer = ctx.tm->layers[ra.tile_layer];
                    layer.rend(ctx, ctx.tile_anim, dim_layer);
                } break;
                default: {
                } break;
            }
        }

        editor.render_after(ctx);

        // It's okay to do GUI stuff from this point on.
        // This is rendered above EVERYTHING else.

        editor.render_gui(ctx);

        gui.end();

        rend.rect(BBox::from(vec2(0, 0) - ctx.cam, cam_max), {0, 255, 0, 255});
        rend.swap_buffers();
    }

    SDL_Quit();
    return 0;
}