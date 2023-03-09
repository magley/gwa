#pragma once

#include "fdecl.h"
#include "platform/input.h"
#include "platform/video.h"
#include "entity/entity.h"
#include "resource/res_mng.h"
#include "tile/tile.h"

struct GwaCtx {
    Input* input;
    Renderer* rend;
    EntityManager* em;
    ResMng* rm;
    TileMap* tm;

    const vec2 view_sz;
    vec2 cam;
    fp6 tile_anim = 0;

    GwaCtx(Input* input, Renderer* rend, EntityManager* em, ResMng* rm, TileMap* tm, const vec2& view_sz):
        input(input), rend(rend), em(em), rm(rm), tm(tm), view_sz(view_sz) {}

    BBox cam_extents() const;
};