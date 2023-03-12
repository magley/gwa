#pragma once

#include "fdecl.h"
#include "platform/input.h"
#include "platform/video.h"
#include "entity/entity.h"
#include "resource/res_mng.h"
#include "tile/tile.h"
#include "gui/gui.h"

struct GwaCtx {
    Input* input;
    Renderer* rend;
    EntityManager* em;
    ResMng* rm;
    TileMap* tm;
    Gui* gui;

    const vec2 view_sz;
    vec2 cam;
    fp6 tile_anim = 0;
    int scale = 3;

    GwaCtx(Input* input, Renderer* rend, EntityManager* em, ResMng* rm, TileMap* tm, Gui* gui, const vec2& view_sz):
        input(input), rend(rend), em(em), rm(rm), tm(tm), gui(gui), view_sz(view_sz) {}

    BBox cam_extents() const;
};