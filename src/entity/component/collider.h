#pragma once

#include "util/geometry/bbox.h"
#include <vector>
#include "entity/typedef.h"

struct cld_c {
    BBox bbox;
    std::vector<EntityID> other; // Others collided with during this frame.

    void build_other(EntityManager& em, EntityID self);
};