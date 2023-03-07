#pragma once

#include "entity/typedef.h"
#include "ctx/fdecl.h"

struct Input;

struct player_c {
    void collect_items(GwaCtx& ctx, EntityID self);
    void move(GwaCtx& ctx, EntityID self);
};