#pragma once

struct Input;
struct GwaCtx;
#include "resource/fwdecl.h"

struct Editor {
    int tile_index = 0;
    int tile_layer = 0;
    bool tile_showgrid = true;
    bool tile_focuslayer = false;

    enum {MODE_TILEMAP};
    int mode = MODE_TILEMAP;

    void add_empty_tile_layer(GwaCtx& ctx);

    void on_input(GwaCtx& ctx);
    void render_before(GwaCtx& ctx);
    void render_after(GwaCtx& ctx);
    void render_gui(GwaCtx& ctx, FontH fnt);

    void on_input_tile(GwaCtx& ctx);
    void render_tile_before(GwaCtx& ctx);
    void render_tile_after(GwaCtx& ctx);
    void render_gui_tile(GwaCtx& ctx, FontH fnt);
};