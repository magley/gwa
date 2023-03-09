#include "rendagent.h"
#include "ctx/ctx.h"

RendAgent RendAgent::from_entity(EntityID e, GwaCtx& ctx) {
    RendAgent a;
    a.entity = e;
    a.type = ENTITY;
    a.depth = ctx.em->body(e)->depth;
    return a;
}

RendAgent RendAgent::from_tilemap(int tilemap_layer, GwaCtx& ctx) {
    RendAgent a;
    a.tile_layer = tilemap_layer;
    a.type = TILE_LAYER;
    a.depth = ctx.tm->layers[a.tile_layer].depth;
    return a;
}

int operator<(const RendAgent& a, const RendAgent& b) {
    if (a.depth < b.depth) return -1;
    if (a.depth > b.depth) return 1;
    return 0;
}