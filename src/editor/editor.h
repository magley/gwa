#pragma once

struct Input;
struct GwaCtx;
#include "resource/fwdecl.h"
#include "util/geometry/bbox.h"
#include "tile/tile.h"

struct Editor {
    int tile_index = 0;
    int tile_layer = 0;
    bool tile_showgrid = true;
    bool tile_focuslayer = false;
    int rect_mode = 0;
    BBox rect;
    TilePos rect_start;

    enum {RECT_NONE, RECT_BEGIN, RECT_START_PLACE, RECT_START_DEL};

    enum {MODE_TILEMAP};
    int mode = MODE_TILEMAP;

    TilePos mouse_cell(GwaCtx& ctx);
    void add_empty_tile_layer(GwaCtx& ctx);
    void place_tile(GwaCtx& ctx, TilePos tile, int tile_index_to_place);

    void on_input(GwaCtx& ctx);
    void render_before(GwaCtx& ctx);
    void render_after(GwaCtx& ctx);
    void render_gui(GwaCtx& ctx, FontH fnt);

    void on_input_tile(GwaCtx& ctx);
    void render_tile_before(GwaCtx& ctx);
    void render_tile_after(GwaCtx& ctx);
    void render_gui_tile(GwaCtx& ctx, FontH fnt);
};