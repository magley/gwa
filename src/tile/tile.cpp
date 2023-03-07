#include "tile.h"
#include "util/geometry/vec2.h"
#include "util/geometry/bbox.h"
#include "ctx/ctx.h"
#include "SDL2/SDL_render.h"

TilePos::TilePos() {

}

TilePos::TilePos(uint16_t x, uint16_t y): x(x), y(y) {
    
}

TileMap::TileMap() {
    
}

std::vector<TilePos> TileMapLayer::touching(const BBox& bbox) const {
    return decompress(touching_compressed(bbox));
}

BBoxDiscrete TileMapLayer::touching_compressed(const BBox& bbox) const {
    const fp6 y_min = bbox.u / sz.y;
    const fp6 y_max = bbox.d / sz.y;
    const fp6 x_min = bbox.l / sz.x;
    const fp6 x_max = bbox.r / sz.x;
    return BBox::from(y_min, x_min, y_max, x_max);
}

std::vector<TilePos> TileMapLayer::decompress(const BBoxDiscrete& bbox) const {
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


void TileMapLayer::rend(GwaCtx& ctx, fp6 anim_frame, bool dim) const {
    Tileset* tst = ctx.rm->tileset(tileset);
    const Texture* txt_tst = ctx.rm->texture(tst->tex);
    const vec2 sz = tst->sz;

    if (dim) {
        SDL_SetTextureAlphaMod(txt_tst->texture, 64);
    }

    for (uint16_t y = 0; y < map.size(); y++) {
        if ((y + sz.y) * sz.y < ctx.cam_extents().u) continue;
        if (y * sz.y > ctx.cam_extents().d) break;
        for (uint16_t x = 0; x < map[y].size(); x++) {
            if ((x + sz.x) * sz.x < ctx.cam_extents().l) continue;
            if (x * sz.x > ctx.cam_extents().r) break;

            const int16_t tindex = map[y][x];
            if (tindex < 0) {
                continue;
            }
            const Tile t = tst->tiles[tindex];

            t.rend(ctx, tst, vec2(x, y), sz, anim_frame);
        }
    }

    SDL_SetTextureAlphaMod(txt_tst->texture, 255);
}

void Tile::rend(GwaCtx& ctx, Tileset* tst, const vec2& cell_pos, const vec2& cell_size, fp6 anim_frame) const {
    int tile_anim_index = (int)(anim_frame % (int)pos.size());

    const fp6 xx = pos[tile_anim_index].x;
    const fp6 yy = pos[tile_anim_index].y;
    const BBox src = BBox::from(vec2(xx, yy), cell_size);

    const vec2 world_pos(
        cell_pos.x * cell_size.x, 
        cell_pos.y * cell_size.y
    );

    ctx.rend->tex(tst->tex, world_pos - ctx.cam, 0, src);
}

void Tileset::clear() {
    tiles.clear();
    tex = -1;
}