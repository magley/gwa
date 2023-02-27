#include "player.h"
#include "entity/entity.h"


void player_c::collect_items(EntityManager& em, EntityID self) {
    if (!em.has(self, CLD)) {
        return;
    }

    cld_c* cld = em.cld(self);

    for (EntityID& o : cld->other) {
        if (!em.has(o, ITEM)) {
            continue;
        }

        item_c* oitem = em.item(o);
        oitem->collect();
    }
}