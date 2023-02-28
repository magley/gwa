#include "item.h"
#include "entity/entity.h"

void item_c::collect() {
    flags |= COLLECTED;
}

void item_c::update(EntityManager& em, EntityID self) {
    if ((flags & COLLECTED) == COLLECTED) {
        em.destroy(self);
    }
}