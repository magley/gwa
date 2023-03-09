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

void Editor::add_empty_tile_layer(GwaCtx& ctx) {
    TileMapLayer tml = TileMapLayer();
    tml.tileset = ctx.rm->tileset("../res/test.tileset");
    tml.map.push_back({-1});
    tml.depth = 10;
    tml.sz = vec2(16, 16);
    ctx.tm->layers.push_back(tml);
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
    Input& input = *ctx.input;

    if (input.press(SDL_SCANCODE_F1)) {
        tile_showgrid ^= true;
    }
    if (input.press(SDL_SCANCODE_F2)) {
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

    if (input.m_down(SDL_BUTTON_LMASK)) {
        TileMapLayer& layer = ctx.tm->layers[tile_layer];
        Tileset* tileset = ctx.rm->tileset(layer.tileset);
        int x = (input.m_x() / 3 + ctx.cam.x) / layer.sz.x;
        int y = (input.m_y() / 3 + ctx.cam.y) / layer.sz.y;
        const Tile t = tileset->tiles[tile_index];

        for (int i = layer.map.size(); i <= y; i++) {
            layer.map.push_back({});
        }
        for (int i = 0; i < layer.map.size(); i++) {
            for (int j = layer.map[i].size(); j <= x; j++) {
                layer.map[i].push_back(-1);
            }
        }

        layer.map[y][x] = tile_index;
    } else if (input.m_down(SDL_BUTTON_RMASK)) {
        TileMapLayer& layer = ctx.tm->layers[tile_layer];
        Tileset* tileset = ctx.rm->tileset(layer.tileset);
        int x = (input.m_x() / 3 + ctx.cam.x) / layer.sz.x;
        int y = (input.m_y() / 3 + ctx.cam.y) / layer.sz.y;
        const Tile t = tileset->tiles[tile_index];

        if (y >= layer.map.size() || x >= layer.map[y].size()) {
            // Do nothing.
        } else {
            layer.map[y][x] = -1;
        }
    }

    if (input.press(SDL_SCANCODE_SPACE)) {
        add_empty_tile_layer(ctx);
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
    const int x = (input.m_x() / 3 + ctx.cam.x) / layer.sz.x;
    const int y = (input.m_y() / 3 + ctx.cam.y) / layer.sz.y;

    t.rend(ctx, tileset, vec2(x, y), layer.sz, ctx.tile_anim);
}

void Editor::render_gui_tile(GwaCtx& ctx, FontH fnt) {
    Renderer& rend = *ctx.rend;
    const std::vector<TileMapLayer>& layers = ctx.tm->layers;
    std::multiset<RendAgent> layers_sorted;
    for (int i = 0; i < layers.size(); i++) {
        layers_sorted.insert(RendAgent::from_tilemap(i, ctx));
    }

    string2 layer_data;
    for (const auto& layer : layers_sorted) {
        layer_data += string2::from(layer.tile_layer) + " " 
                    + string2::from(layer.depth);
        if (layer.tile_layer == tile_layer) {
            layer_data += "* ";
        }
        layer_data += "\n";
    }
    rend.text(vec2(8, 8), fnt, layer_data);
}