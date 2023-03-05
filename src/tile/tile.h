#pragma once
#include <stdint.h>
#include <vector>
#include "util/geometry/vec2.h"
#include "fdecl.h"
#include "resource/fwdecl.h"
#include "ctx/fdecl.h"

struct string2;
struct BBox;

struct Tile {
    uint8_t v;
    Tile(uint8_t v): v(v) {};
};

struct TilePos {
    uint16_t x, y;
    TilePos();
    TilePos(uint16_t x, uint16_t y);
};

struct Tileset {
    TextureH tex;
    std::vector<Tile> tiles;
    vec2 sz;

    Tileset() {}
    Tileset(TextureH tex, std::vector<Tile> tiles, vec2 sz): tex(tex), 
        tiles(tiles), sz(sz) {}

    string2 save(GwaCtx& ctx) const;
    void load(GwaCtx& ctx, const string2& s);
    
    void clear();
};

struct TileMap {
    TilesetH tileset;
    std::vector<std::vector<uint16_t>> map;
    vec2 sz;

    TileMap();

    std::vector<TilePos> touching(const BBox& bbox) const;
    BBoxDiscrete touching_compressed(const BBox& bbox) const;
    std::vector<TilePos> decompress(const BBoxDiscrete& bbox) const;

    string2 save(GwaCtx& ctx) const;
    void load(GwaCtx& ctx, const string2& s);
};