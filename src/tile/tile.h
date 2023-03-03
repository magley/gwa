#pragma once
#include <stdint.h>
#include <vector>
#include "util/geometry/vec2.h"
#include "fdecl.h"

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

struct TileMap {
    std::vector<Tile> tileset;
    std::vector<std::vector<uint16_t>> map;
    vec2 sz;

    TileMap();

    std::vector<TilePos> touching(const BBox& bbox) const;
    BBoxDiscrete touching_compressed(const BBox& bbox) const;
    std::vector<TilePos> decompress(const BBoxDiscrete& bbox) const;
};