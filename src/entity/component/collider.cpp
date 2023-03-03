#include "collider.h"
#include "entity/entity.h"
#include "tile/tile.h"

void cld_c::solve(fp6 curr, fp6 desired, fp6* in) const {
    *in += desired - curr;
}

void cld_c::build_other(EntityManager& em, EntityID self) {
    other.clear();

    body_c* body = em.body(self);
    phys_c* phys = em.phys(self);
    
    vec2 abs_vel = vec2();
    if (em.has(self, PHYS)) {
        abs_vel = phys->v.abs();
    }

    const BBox bbox_self = (bbox + body->p).exp(abs_vel).exp(vec2(1, 1));

    for (EntityID e : em.get_all(CLD)) {
        cld_c* ocld = em.cld(e);
        if (ocld == this) {
            continue;
        }

        body_c* obody = em.body(e);
        phys_c* ophys = em.phys(e);

        abs_vel = vec2();
        if (em.has(e, PHYS)) {
            abs_vel = ophys->v.abs();
        }

        const BBox bbox_other = (ocld->bbox + obody->p).exp(abs_vel);

        if (bbox_self.cld(bbox_other)) {
            other.push_back(e);
        }
    }
}

void cld_c::build_tilemap_range(EntityManager& em, EntityID self, const TileMap& tm) {
    body_c* body = em.body(self);
    phys_c* phys = em.phys(self);
    vec2 abs_vel = vec2();
    if (em.has(self, PHYS)) {
        abs_vel = phys->v.abs();
    }
    
    const BBox bbox_self = (bbox + body->p).exp(abs_vel).exp(vec2(1, 1));
    tilemap_range = tm.touching_compressed(bbox_self);
}

bool cld_c::collision(EntityManager& em, EntityID self, EntityID other) {
    body_c* body = em.body(self);
    body_c* obody = em.body(other);
    cld_c* ocld = em.cld(other);

    BBox b1 = bbox + body->p;
    BBox b2 = ocld->bbox + obody->p;

    return b1.cld(b2);
}

bool cld_c::collision_excl(EntityManager& em, EntityID self, EntityID other) {
    body_c* body = em.body(self);
    body_c* obody = em.body(other);
    cld_c* ocld = em.cld(other);

    BBox b1 = bbox + body->p;
    BBox b2 = ocld->bbox + obody->p;

    return b1.cld_exc(b2);
}