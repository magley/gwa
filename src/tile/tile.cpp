#include "tile.h"
#include "util/geometry/vec2.h"
#include "util/geometry/bbox.h"

TilePos::TilePos() {

}

TilePos::TilePos(uint16_t x, uint16_t y): x(x), y(y) {
    
}

TileMap::TileMap() {
    
}

std::vector<TilePos> TileMap::touching(const BBox& bbox) const {
    return decompress(touching_compressed(bbox));
}

BBoxDiscrete TileMap::touching_compressed(const BBox& bbox) const {
    const fp6 y_min = bbox.u / sz.y;
    const fp6 y_max = bbox.d / sz.y;
    const fp6 x_min = bbox.l / sz.x;
    const fp6 x_max = bbox.r / sz.x;
    return BBox::from(y_min, x_min, y_max, x_max);
}

std::vector<TilePos> TileMap::decompress(const BBoxDiscrete& bbox) const {
    int16_t y_min = (int16_t)bbox.u.clamp(-1, (int)map.size());
    int16_t y_max = (int16_t)bbox.d.clamp(-1, (int)map.size());
    int16_t x_min = (int16_t)bbox.l.clamp(-1, (int)map[0].size());
    int16_t x_max = (int16_t)bbox.r.clamp(-1, (int)map[0].size());

    std::vector<TilePos> res;
    for (int16_t y = y_min; y <= y_max; y++) {
        if (y < 0) {
            continue;
        }
        if (y >= map.size()) {
            break;
        }
        for (int16_t x = x_min; x <= x_max; x++) {
            if (x < 0) {
                continue;
            }
            if (x >= map[y].size()) {
                break;
            }
            res.push_back(TilePos(x, y));
        }
    }

    return res; 
}

void Tileset::clear() {
    tiles.clear();
    tex = -1;
}