#pragma once

#include "util/geometry/bbox.h"
#include <vector>
#include "entity/typedef.h"

struct cld_c {
    BBox bbox;
    std::vector<EntityID> other; // Others collided with during this frame.
    enum : uint8_t {
        NONE = 0b0000,
        SOLID = 0b0001,
    };
    
    uint8_t flags;

    void solve(fp6 curr, fp6 desired, fp6* in) const;

    void build_other(EntityManager& em, EntityID self);
};