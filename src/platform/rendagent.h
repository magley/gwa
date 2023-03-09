#pragma once

#include "entity/typedef.h"
#include "ctx/fdecl.h"

struct RendAgent {
    union {
        EntityID entity;
        int tile_layer;
    };
    enum { ENTITY, TILE_LAYER };
    int type;
    int8_t depth;

    static RendAgent from_entity(EntityID e, GwaCtx& ctx);
    static RendAgent from_tilemap(int tilemap_layer, GwaCtx& ctx);
};

int operator<(const RendAgent& a, const RendAgent& b);