#pragma once

#include "entity/typedef.h"

struct item_c {
    enum : int8_t {
        NONE = 0b0000,
        COLLECTED = 0b0001,
    };
    int8_t flags;

    void collect();
    void update(EntityManager& em, EntityID self);
};