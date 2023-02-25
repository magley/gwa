#pragma once

#include "util/geometry/vec2.h"
#include "entity/typedef.h"

struct phys_c {
    vec2 v;
    fp6 angvel;

    void move(EntityManager& em, EntityID self);
    void cld_solid(EntityManager& em, EntityID self);

    
};
