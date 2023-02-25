#include "physics.h"
#include "entity/entity.h"

void phys_c::move(EntityManager& em, EntityID self) {
    body_c* body = em.body(self);

    body->p += v;
    body->ang += angvel;
}

struct cld_bundle {
    fp6 val;
    bool use_smaller;
    EntityID other = ENTITY_NULL;

    cld_bundle(bool use_smaller): use_smaller(use_smaller) {
        val = use_smaller ? 2147483647 : -2147483648;
    }

    void send(fp6 newval, EntityID o) {
        if ((use_smaller && newval <= val) || (newval >= val)) {
            val = newval;
            other = o;
        }
    }

    bool found() const {
        return other != ENTITY_NULL;
    }
};

void phys_c::cld_solid(EntityManager& em, EntityID self) {
    body_c* body = em.body(self);
    cld_c* cld = em.cld(self);
    const BBox bbox = cld->bbox + body->p;

    cld_bundle cb_u(true);
    cld_bundle cb_d(false);
    cld_bundle cb_l(true);
    cld_bundle cb_r(false);

    // Floor

    for (EntityID o : cld->other) {
        cld_c* ocld = em.cld(o);
        if ((ocld->flags & cld_c::SOLID) != cld_c::SOLID) {
            continue;
        }

        body_c* obody = em.body(o);
        phys_c* ophys = em.phys(o);
        const BBox obbox = ocld->bbox + obody->p;
        const fp6 target_pos = obbox.u;

        if (!bbox.cld_h(obbox.exp(vec2(-1, -1)))) {
            continue;
        }

        if (bbox.d <= target_pos && bbox.d + v.y >= target_pos) {
            cb_u.send(target_pos, o);
        }
    }

    if (cb_u.found()) {
        cld->solve(bbox.d, cb_u.val, &body->p.y);
        v.y = 0;
    }

    // Ceiling

    for (EntityID o : cld->other) {
        cld_c* ocld = em.cld(o);
        if ((ocld->flags & cld_c::SOLID) != cld_c::SOLID) {
            continue;
        }

        body_c* obody = em.body(o);
        phys_c* ophys = em.phys(o);
        const BBox obbox = ocld->bbox + obody->p;
        const fp6 target_pos = obbox.d;

        if (!bbox.cld_h(obbox.exp(vec2(-1, -1)))) {
            continue;
        }

        if (bbox.u >= target_pos && bbox.u + v.y <= target_pos) {
            cb_d.send(target_pos, o);
        }
    }

    if (cb_d.found()) {
        cld->solve(bbox.u, cb_d.val, &body->p.y);
        v.y = 0;
    }

    // Rwall

    for (EntityID o : cld->other) {
        cld_c* ocld = em.cld(o);
        if ((ocld->flags & cld_c::SOLID) != cld_c::SOLID) {
            continue;
        }

        body_c* obody = em.body(o);
        phys_c* ophys = em.phys(o);
        const BBox obbox = ocld->bbox + obody->p;
        const fp6 target_pos = obbox.l;

        if (!bbox.cld_v(obbox.exp(vec2(-1, -1)))) {
            continue;
        }

        if (bbox.r <= target_pos && bbox.r + v.x >= target_pos) {
            cb_l.send(target_pos, o);
        }
    }

    if (cb_l.found()) {
        cld->solve(bbox.r, cb_l.val, &body->p.x);
        v.x = 0;
    }

    // Lwall

    for (EntityID o : cld->other) {
        cld_c* ocld = em.cld(o);
        if ((ocld->flags & cld_c::SOLID) != cld_c::SOLID) {
            continue;
        }

        body_c* obody = em.body(o);
        phys_c* ophys = em.phys(o);
        const BBox obbox = ocld->bbox + obody->p;
        const fp6 target_pos = obbox.r;

        if (!bbox.cld_v(obbox.exp(vec2(-1, -1)))) {
            continue;
        }

        if (bbox.l >= target_pos && bbox.l + v.x <= target_pos) {
            cb_r.send(target_pos, o);
        }
    }

    if (cb_r.found()) {
        cld->solve(bbox.l, cb_r.val, &body->p.x);
        v.x = 0;
    }
}