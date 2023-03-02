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
    const uint16_t y_min = (uint16_t)(bbox.u / sz.y);
    const uint16_t y_max = (uint16_t)(bbox.d / sz.y);
    const uint16_t x_min = (uint16_t)(bbox.l / sz.x);
    const uint16_t x_max = (uint16_t)(bbox.r / sz.x);

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