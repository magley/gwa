#include "physics.h"
#include "entity/entity.h"

void phys_c::move(EntityManager& em, EntityID self) {
    body_c* body = em.body(self);

    body->p += im;
    body->p += v;
    body->ang += angvel;

    im = {0, 0};
}

struct cld_bundle {
    fp6 val;
    fp6 ovel;
    vec2 ovel_full;
    int cmp_dir;
    EntityID other = ENTITY_NULL;

    cld_bundle(bool use_smaller): cmp_dir(use_smaller ? 1 : -1) {
        val = use_smaller ? FP6_MAX : FP6_MIN;
    }

    void send(fp6 newval, fp6 o_vel, EntityID o, vec2 o_vel_full) {
        if ((newval + o_vel) * cmp_dir <= val * cmp_dir) {
            val = newval;
            other = o;
            ovel = o_vel;
            ovel_full = o_vel_full;
        }
    }

    bool found() const {
        return other != ENTITY_NULL;
    }
};

void phys_c::cld_solid(EntityManager& em, EntityID self) {
    body_c* body = em.body(self);
    cld_c* cld = em.cld(self);
    BBox bbox;

    cld_bundle cb_u(true);
    cld_bundle cb_d(false);
    cld_bundle cb_l(true);
    cld_bundle cb_r(false);

    // Floor

    bbox = cld->bbox + body->p;
    for (EntityID o : cld->other) {
        cld_c* ocld = em.cld(o);
        if ((ocld->flags & cld_c::SOLID) != cld_c::SOLID) {
            continue;
        }

        body_c* obody = em.body(o);
        phys_c* ophys = em.phys(o);
        const BBox obbox = ocld->bbox + obody->p;
        const fp6 target_pos = obbox.u;

        const vec2 ov = em.has(o, PHYS) ? ophys->v : vec2(0, 0);
        if (!(bbox).cld_h_exc((obbox).exp(vec2(0, -1)))) {
            continue;
        }

        const fp6 dv = v.y - ov.y;
        const fp6 dp_sicky = 1.5;

        if (
            bbox.d <= target_pos && (
                (dv >= 0 && bbox.d + dv >= target_pos) ||
                (dv < 0 && ov.y > 0 && (bbox.d + dv - target_pos >= -dp_sicky))
            )
        ) {
            cb_u.send(target_pos, ov.y, o, ov);
        }
    }

    if (cb_u.found()) {
        cld->solve(bbox.d, cb_u.val, &body->p.y);
        v.y = 0;
        im += cb_u.ovel_full;
    }

    // Ceiling

    bbox = cld->bbox + body->p;
    for (EntityID o : cld->other) {
        cld_c* ocld = em.cld(o);
        if ((ocld->flags & cld_c::SOLID) != cld_c::SOLID) {
            continue;
        }

        body_c* obody = em.body(o);
        phys_c* ophys = em.phys(o);
        const BBox obbox = ocld->bbox + obody->p;
        const fp6 target_pos = obbox.d;

        const vec2 ov = em.has(o, PHYS) ? ophys->v : vec2(0, 0);
        if (!(bbox).cld_h_exc((obbox).exp(vec2(0, -1)))) {
            continue;
        }

        const fp6 dv = v.y - ov.y;

        if (dv <= 0 && bbox.u >= target_pos && bbox.u + dv <= target_pos) {
            cb_d.send(target_pos, ov.y, o, ov);
        }
    }

    if (cb_d.found()) {
        cld->solve(bbox.u, cb_d.val, &body->p.y);
        v.y = 0;
        im.y += cb_d.ovel;
    }

    // Rwall

    bbox = cld->bbox + body->p;
    for (EntityID o : cld->other) {
        cld_c* ocld = em.cld(o);
        if ((ocld->flags & cld_c::SOLID) != cld_c::SOLID) {
            continue;
        }

        body_c* obody = em.body(o);
        phys_c* ophys = em.phys(o);
        const BBox obbox = ocld->bbox + obody->p;
        const fp6 target_pos = obbox.l;

        const vec2 ov = em.has(o, PHYS) ? ophys->v : vec2(0, 0);
        if (!(bbox + (v + im - ov)).cld_v_exc((obbox).exp(vec2(0, 0)))) {
            continue;
        }

        const fp6 dv = v.x + im.x - ov.x;

        if (dv >= 0 && bbox.r <= target_pos && bbox.r + dv >= target_pos) {
            cb_l.send(target_pos, ov.x, o, ov);
        }
    }

    if (cb_l.found()) {
        cld->solve(bbox.r, cb_l.val, &body->p.x);
        v.x = 0;

        if (cb_l.ovel < 0) {
            im.x += cb_l.ovel;
        } else {
            if (im.x > 0) {
                im.x = 0;
            }
        }
    }

    // Lwall

    bbox = cld->bbox + body->p;
    for (EntityID o : cld->other) {
        cld_c* ocld = em.cld(o);
        if ((ocld->flags & cld_c::SOLID) != cld_c::SOLID) {
            continue;
        }

        body_c* obody = em.body(o);
        phys_c* ophys = em.phys(o);
        const BBox obbox = ocld->bbox + obody->p;
        const fp6 target_pos = obbox.r;

        const vec2 ov = em.has(o, PHYS) ? ophys->v : vec2(0, 0);
        if (!(bbox + (v + im - ov)).cld_v_exc((obbox).exp(vec2(-1, 0)))) {
            continue;
        }

        const fp6 dv = v.x - ov.x;

        if (dv <= 0 && bbox.l >= target_pos && bbox.l + dv <= target_pos) {
            cb_r.send(target_pos, ov.x, o, ov);
        }
    }

    if (cb_r.found()) {
        cld->solve(bbox.l, cb_r.val, &body->p.x);
        v.x = 0;

        if (cb_r.ovel > 0) {
            im.x += cb_r.ovel;
        } else {
            if (im.x < 0) {
                im.x = 0;
            }
        }
    }
}