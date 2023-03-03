#pragma once

#include "util/geometry/vec2.h"
#include "entity/typedef.h"
#include "tile/fdecl.h"

struct phys_c {
    vec2 v;
    vec2 im;
    fp6 angvel;
    enum : uint8_t {
        NONE = 0b0000,
        CLD_SOLID = 0b0001,
    };
    
    uint8_t flags;

    void move(EntityManager& em, EntityID self);
    void cld_solid(EntityManager& em, EntityID self, const TileMap& tm);
};
