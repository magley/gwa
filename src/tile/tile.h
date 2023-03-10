#pragma once
#include <stdint.h>
#include <vector>
#include "util/geometry/vec2.h"
#include "fdecl.h"
#include "resource/fwdecl.h"
#include "ctx/fdecl.h"

struct string2;
struct BBox;
struct SDL_Color;

struct Tile {
    uint8_t v;
    std::vector<vec2> pos;
    Tile(uint8_t v, const std::vector<vec2>& pos): v(v), pos(pos) {};

    void rend(GwaCtx& ctx, Tileset* tst, const vec2& cell_pos, const vec2& cell_size, fp6 anim_frame, SDL_Color colormod) const;

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

struct TileMapLayer {
    int depth;
    std::vector<std::vector<int16_t>> map;
    vec2 sz;
    TilesetH tileset;

    std::vector<TilePos> touching(const BBox& bbox) const;
    BBoxDiscrete touching_compressed(const BBox& bbox) const;
    std::vector<TilePos> decompress(const BBoxDiscrete& bbox) const;

    string2 save(GwaCtx& ctx) const;
    void load(GwaCtx& ctx, const string2& s);

    void rend(GwaCtx& ctx, fp6 anim_frame, bool dim) const;
};

struct TileMap {
    std::vector<TileMapLayer> layers;

    TileMap();

    string2 save(GwaCtx& ctx) const;
    void load(GwaCtx& ctx, const string2& s);
};