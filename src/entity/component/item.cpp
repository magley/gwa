#include "item.h"
#include "entity/entity.h"
#include "ctx/ctx.h"

void item_c::collect() {
    flags |= COLLECTED;
}

void item_c::update(GwaCtx& ctx, EntityID self) {
    if ((flags & COLLECTED) == COLLECTED) {
        ctx.em->destroy(self);
    }
}