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

void rend_tilemap_layer(const TileMapLayer& layer, GwaCtx& ctx, BBox cam_extents, fp6 tile_anim_frame, const vec2& cam, bool should_dim) {
    Tileset* tileset = ctx.rm->tileset(layer.tileset);
    const Texture* txt_tst = ctx.rm->texture(tileset->tex);
    const vec2 sz = tileset->sz;

    if (should_dim) {
        SDL_SetTextureAlphaMod(txt_tst->texture, 64);
    }

    for (uint16_t y = 0; y < layer.map.size(); y++) {
        if ((y + layer.sz.y) * layer.sz.y < cam_extents.u) continue;
        if (y * layer.sz.y > cam_extents.d) break;
        for (uint16_t x = 0; x < layer.map[y].size(); x++) {
            if ((x + layer.sz.x) * layer.sz.x < cam_extents.l) continue;
            if (x * layer.sz.x > cam_extents.r) break;

            const int16_t tindex = layer.map[y][x];
            if (tindex < 0) {
                continue;
            }
            const Tile t = tileset->tiles[tindex];
            const uint8_t v = t.v;

            int tile_anim_index = (int)(tile_anim_frame % (int)t.pos.size());

            const fp6 xx = t.pos[tile_anim_index].x;
            const fp6 yy = t.pos[tile_anim_index].y;
            const BBox src = BBox::from(vec2(xx, yy), sz);
            ctx.rend->tex(tileset->tex, vec2(x * layer.sz.x, y * layer.sz.y) - cam, 0, src);
        }
    }

    SDL_SetTextureAlphaMod(txt_tst->texture, 255);
}

void rend_entity(EntityID e, GwaCtx& ctx, const vec2& cam) {
    if (ctx.em->has(e, CLD)) {
        body_c* body = ctx.em->body(e);
        cld_c* cld = ctx.em->cld(e);

        ctx.rend->rect(cld->bbox + (body->p - cam), {180, 180, 180, 255});
        if (cld->other.size() == 0) {
        } else {
            ctx.rend->rectf(cld->bbox + (body->p - cam), {32, 255, 96, 96});
        }
    }

    if (ctx.em->has(e, SPR)) {
        body_c* body = ctx.em->body(e);
        spr_c* spr = ctx.em->spr(e);
        TextureH tex = spr->tex;

        if (ctx.em->has(e, CLD)) {
            cld_c* cld = ctx.em->cld(e);
            ctx.rend->tex_sized(tex, body->p - cam, 0, cld->bbox.size());
        } else {
            ctx.rend->tex(tex, body->p - cam, 0);
        }
    }
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

    GwaCtx ctx(&input, &rend, &em, &res_mng, &tm);
    res_mng.init_ctx(&ctx);
    fp6 tile_anim_frame = 0;
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

    vec2 cam;
    const vec2 cam_max = vec2(
        view_w * 2,
        view_h * 1
    );
    const vec2 view_sz = vec2(view_w, view_h);

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
            cam = body->p - view_sz / 2;
            cam.x = cam.x.clamp(0, cam_max.x - view_sz.x);
            cam.y = cam.y.clamp(0, cam_max.y - view_sz.y);
            break;
        }

        tile_anim_frame += tile_anim_spd;

        ///// UPDATE EDITOR
        {
            if (input.m_down(SDL_BUTTON_LMASK)) {
                TileMapLayer& layer = ctx.tm->layers[editor_focused_tile_layer];
                Tileset* tileset = ctx.rm->tileset(layer.tileset);
                int x = (input.m_x() / 3 + cam.x) / layer.sz.x;
                int y = (input.m_y() / 3 + cam.y) / layer.sz.y;
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

        const BBox cam_extents = BBox::from(cam, view_sz);
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
                    const fp6 y_ = (int)((yy + cam.y) / layer.sz.y) * layer.sz.y;
                    rend.line(vec2(cam_extents.l, y_) - cam, vec2(cam_extents.r, y_) - cam, {255, 255, 255, 64});
                }

                for (fp6 xx = 0; xx <= view_w + layer.sz.x; xx += layer.sz.x) {
                    const fp6 x_ = (int)((xx + cam.x) / layer.sz.x) * layer.sz.x;
                    rend.line(vec2(x_, cam_extents.u) - cam, vec2(x_, cam_extents.d) - cam, {255, 255, 255, 64});
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
                    rend_entity(ra.entity, ctx, cam);
                } break;
                case RendAgent::TILE_LAYER: {
                    bool should_dim_layer = editor_dim_unfocused_layers && (editor_focused_tile_layer != ra.tile_layer);
                    rend_tilemap_layer(ctx.tm->layers[ra.tile_layer], ctx, cam_extents, tile_anim_frame, cam, should_dim_layer);
                } break;
                default: {

                } break;
            }
        }

        /////// RENDER EDITOR AFTER
        {
            TileMapLayer& layer = ctx.tm->layers[editor_focused_tile_layer];
            Tileset* tileset = ctx.rm->tileset(layer.tileset);

            // TODO: This and the general-purpose tile rend use the same code.
            // Except for the logic in deciding which Tile t to choose and x,y.

            int x = (input.m_x() / 3 + cam.x) / layer.sz.x;
            int y = (input.m_y() / 3 + cam.y) / layer.sz.y;

            const Tile t = tileset->tiles[editor_tile_index];

            int tile_anim_index = (int)(tile_anim_frame % (int)t.pos.size());

            const fp6 xx = t.pos[tile_anim_index].x;
            const fp6 yy = t.pos[tile_anim_index].y;
            const BBox src = BBox::from(vec2(xx, yy), tileset->sz);
            ctx.rend->tex(tileset->tex, vec2(x * layer.sz.x, y * layer.sz.y) - cam, 0, src);
        }
        ///////////////////////////////////

        rend.rect(BBox::from(vec2(0, 0) - cam, cam_max), {0, 255, 0, 255});
        rend.swap_buffers();
    }

    SDL_Quit();
    return 0;
}