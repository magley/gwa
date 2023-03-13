#include "editor.h"
#include "ctx/ctx.h"
#include "platform/input.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_mouse.h"
#include "platform/rendagent.h"
#include <set>

int wrap(int a, int mini, int maxi) {
    if (a < 0) {
        return maxi;
    } else if (a > maxi) {
        return mini;
    }
    return a;
}

//=============================================================================
// OTHER FUNCS
//=============================================================================

TilePos Editor::mouse_cell(GwaCtx& ctx) {
    const TileMapLayer& layer = ctx.tm->layers[tile_layer];
    const int x = (ctx.input->m_x() / 3 + ctx.cam.x) / layer.sz.x;
    const int y = (ctx.input->m_y() / 3 + ctx.cam.y) / layer.sz.y;
    return TilePos(x, y);
}

void Editor::add_empty_tile_layer(GwaCtx& ctx) {
    TileMapLayer tml = TileMapLayer();
    tml.tileset = ctx.rm->tileset("../res/test.tileset");
    tml.map.push_back({-1});
    tml.depth = 10;
    tml.sz = vec2(16, 16);
    ctx.tm->layers.push_back(tml);
}

void Editor::place_tile(GwaCtx& ctx, TilePos tile, int tile_index_to_place) {
    Input& input = *ctx.input;

    TileMapLayer& layer = ctx.tm->layers[tile_layer];
    Tileset* tileset = ctx.rm->tileset(layer.tileset);
    const Tile t = tileset->tiles[tile_index];

    if (tile_index_to_place >= 0) {
        for (int i = layer.map.size(); i <= tile.y; i++) {
            layer.map.push_back({});
        }
        for (int i = 0; i < layer.map.size(); i++) {
            for (int j = layer.map[i].size(); j <= tile.x; j++) {
                layer.map[i].push_back(-1);
            }
        }
    } else {
        if (tile.y >= layer.map.size() || tile.x >= layer.map[tile.y].size()) {
            return; // When deleting no need to expand with empty space.
        }
    }

    layer.map[tile.y][tile.x] = tile_index_to_place;
}

//=============================================================================
// HIGH-LEVEL FUNCS
//=============================================================================

void Editor::on_input(GwaCtx& ctx) {
    switch (mode) {
        case MODE_TILEMAP:
            on_input_tile(ctx);
            break;
        default:
            break;
    }
}

void Editor::render_before(GwaCtx& ctx) {
    switch (mode) {
        case MODE_TILEMAP:
            render_tile_before(ctx);
            break;
        default:
            break;
    }
}

void Editor::render_after(GwaCtx& ctx) {
    switch (mode) {
        case MODE_TILEMAP:
            render_tile_after(ctx);
            break;
        default:
            break;
    }
}

void Editor::render_gui(GwaCtx& ctx, FontH fnt) {
    switch (mode) {
        case MODE_TILEMAP:
            render_gui_tile(ctx, fnt);
            break;
        default:
            break;
    }
}

//=============================================================================
// TILE FUNCS
//=============================================================================

void Editor::on_input_tile(GwaCtx& ctx) {
    if (ctx.gui->hasfocus()) {
        return;
    }

    Input& input = *ctx.input;

    if (input.press(SDL_SCANCODE_1)) {
        tile_showgrid ^= true;
    }
    if (input.press(SDL_SCANCODE_2)) {
        tile_focuslayer ^= true;
    }

    tile_layer += input.press(SDL_SCANCODE_EQUALS) - input.press(SDL_SCANCODE_MINUS);
    tile_layer = wrap(tile_layer, 0, ctx.tm->layers.size() - 1);

    tile_index += input.m_w();

    if (ctx.tm->layers.size() > 0) {
        tile_index = wrap(
            tile_index, 
            0, 
            ctx.rm->tileset(ctx.tm->layers[tile_layer].tileset)->tiles.size() - 1
        );
    }

    if (rect_mode == RECT_NONE) {
        if (input.m_down(SDL_BUTTON_LMASK)) {
            place_tile(ctx, mouse_cell(ctx), tile_index);
        } else if (input.m_down(SDL_BUTTON_RMASK)) {
            place_tile(ctx, mouse_cell(ctx), -1);
        }

        if (input.press(SDL_SCANCODE_SPACE)) {
            add_empty_tile_layer(ctx);
        }

        if (input.down(SDL_SCANCODE_LCTRL)) {
            rect_mode = RECT_BEGIN;
            rect_start = mouse_cell(ctx);
            rect.u = rect_start.y;
            rect.l = rect_start.x;
            rect.d = rect_start.y + 1;
            rect.r = rect_start.x + 1;
        }
    } else if (rect_mode != RECT_NONE) {
        const TilePos mp = mouse_cell(ctx);

        rect.r = mp.x + (mp.x < rect.l ? 0 : 1);
        rect.d = mp.y + (mp.y < rect.u ? 0 : 1);
        rect.l = rect_start.x + (mp.x < rect.l ? 1 : 0);
        rect.u = rect_start.y + (mp.y < rect.u ? 1 : 0);

        if (input.release(SDL_SCANCODE_LCTRL)) {
            rect_mode = RECT_NONE;
        }

        if (input.m_press(SDL_BUTTON_LMASK)) {
            rect_mode = RECT_START_PLACE;
        } else if (input.m_press(SDL_BUTTON_RMASK)) {
            rect_mode = RECT_START_DEL;
        }

        if (input.m_release(SDL_BUTTON_LMASK) && rect_mode == RECT_START_PLACE) {
            rect_mode = RECT_NONE;

            for (int y = min(rect.u, rect.d); y < max(rect.u, rect.d); y++) {
                for (int x = min(rect.l, rect.r); x < max(rect.l, rect.r); x++) {
                    place_tile(ctx, TilePos(x, y), tile_index);  
                }
            }
        }
        if (input.m_release(SDL_BUTTON_RMASK) && rect_mode == RECT_START_DEL) {
            rect_mode = RECT_NONE;

            for (int y = min(rect.u, rect.d); y < max(rect.u, rect.d); y++) {
                for (int x = min(rect.l, rect.r); x < max(rect.l, rect.r); x++) {
                    place_tile(ctx, TilePos(x, y), -1);  
                }
            }
        }
    }
}

void Editor::render_tile_before(GwaCtx& ctx) {
    TileMapLayer& layer = ctx.tm->layers[tile_layer];
    Renderer& rend = *ctx.rend;

    if (tile_showgrid) {
        for (fp6 yy = 0; yy <= ctx.view_sz.y + layer.sz.y; yy += layer.sz.y) {
            const fp6 y_ = (int)((yy + ctx.cam.y) / layer.sz.y) * layer.sz.y;
            rend.line(vec2(ctx.cam_extents().l, y_) - ctx.cam, vec2(ctx.cam_extents().r, y_) - ctx.cam, {255, 255, 255, 64});
        }

        for (fp6 xx = 0; xx <= ctx.view_sz.x + layer.sz.x; xx += layer.sz.x) {
            const fp6 x_ = (int)((xx + ctx.cam.x) / layer.sz.x) * layer.sz.x;
            rend.line(vec2(x_, ctx.cam_extents().u) - ctx.cam, vec2(x_, ctx.cam_extents().d) - ctx.cam, {255, 255, 255, 64});
        }
    }   
}

void Editor::render_tile_after(GwaCtx& ctx) {
    Input& input = *ctx.input;

    TileMapLayer& layer = ctx.tm->layers[tile_layer];
    Tileset* tileset = ctx.rm->tileset(layer.tileset);
    const Tile t = tileset->tiles[tile_index];

    if (rect_mode != RECT_NONE) {
        BBox bbox_world = rect;
        bbox_world.l *= layer.sz.x;
        bbox_world.r *= layer.sz.x;
        bbox_world.u *= layer.sz.y;
        bbox_world.d *= layer.sz.y;
        bbox_world += -ctx.cam;
        ctx.rend->rect(bbox_world, {255, 0, 0, 255});

        for (int y = min(rect.u, rect.d); y < max(rect.u, rect.d); y++) {
            for (int x = min(rect.l, rect.r); x < max(rect.l, rect.r); x++) {
                t.rend(ctx, tileset, vec2(x, y), layer.sz, ctx.tile_anim, {100, 255, 255, 128});
            }
        }
    } else if (!ctx.gui->hasfocus()) {
        TilePos p = mouse_cell(ctx);
        t.rend(ctx, tileset, vec2(p.x, p.y), layer.sz, ctx.tile_anim, {100, 255, 255, 128});
    }
}

void Editor::render_gui_tile(GwaCtx& ctx, FontH fnt) {
    Renderer& rend = *ctx.rend;
    Gui& gui = *ctx.gui;

    const std::vector<TileMapLayer>& layers = ctx.tm->layers;
    std::multiset<RendAgent> layers_sorted;
    for (int i = 0; i < layers.size(); i++) {
        layers_sorted.insert(RendAgent::from_tilemap(i, ctx));
    }

    gui.startwin("MyWindow1", vec2(96, 32), vec2(128, 64));
    gui.checkbox(vec2(4, 8), {7, 7}, &this->tile_showgrid);
    gui.checkbox(vec2(20, 8),{7, 7}, &this->tile_focuslayer);
    if (gui.button(vec2(40, 8), "Add layer")) {
        add_empty_tile_layer(ctx);
    }
    gui.endwin();

    string2 layer_data;
    for (const auto& layer : layers_sorted) {
        layer_data += string2::from(layer.tile_layer) + " " 
                    + string2::from(layer.depth);
        if (layer.tile_layer == tile_layer) {
            layer_data += "* ";
        }
        layer_data += "\n";
    }
    rend.text(vec2(4, 24), fnt, layer_data);

}