#pragma once

#include "entity/typedef.h"

struct player_c {
    void collect_items(EntityManager& em, EntityID self);
};