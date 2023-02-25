#include "physics.h"
#include "entity/entity.h"

void phys_c::move(EntityManager& em, EntityID self) {
    body_c* body = em.body(self);

    body->p += v;
    body->ang += angvel;
}