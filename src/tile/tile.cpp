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
    const uint16_t y_min = (uint16_t)max(0, bbox.u);
    const uint16_t y_max = (uint16_t)min((int)map.size(), bbox.d);
    const uint16_t x_min = (uint16_t)max(0, bbox.l);
    const uint16_t x_max = (uint16_t)min((int)map[0].size(), bbox.r);

    std::vector<TilePos> res;
    for (uint16_t y = y_min; y <= y_max; y++) {
        if (y > map.size()) {
            break;
        }
        for (uint16_t x = x_min; x <= x_max; x++) {
            if (x > map[y].size()) {
                break;
            }
            res.push_back(TilePos(x, y));
        }
    }

    return res; 
}