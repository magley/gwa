#pragma once
#include <stdint.h>
#include <vector>
#include "util/geometry/vec2.h"

struct BBox;

struct Tile {
    int8_t v;
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
};