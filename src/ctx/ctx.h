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

    GwaCtx(Input* input, Renderer* rend, EntityManager* em, ResMng* rm, TileMap* tm):
        input(input), rend(rend), em(em), rm(rm), tm(tm)
    {

    }
};