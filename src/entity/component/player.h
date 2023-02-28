#pragma once

#include "entity/typedef.h"

struct Input;

struct player_c {
    void collect_items(EntityManager& em, EntityID self);
    void move(EntityManager& em, EntityID self, const Input& input);
};