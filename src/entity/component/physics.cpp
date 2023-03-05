#include "physics.h"
#include "entity/entity.h"
#include "tile/tile.h"
#include "ctx/ctx.h"

void phys_c::move(GwaCtx& ctx, EntityID self) {
    body_c* body = ctx.em->body(self);

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
    bool did_found = false;
    //EntityID other = ENTITY_NULL;

    cld_bundle(bool use_smaller): cmp_dir(use_smaller ? 1 : -1) {
        val = use_smaller ? FP6_MAX : FP6_MIN;
    }

    void send(fp6 newval, fp6 o_vel, vec2 o_vel_full) {
        if ((newval + o_vel) * cmp_dir <= val * cmp_dir) {
            val = newval;
            did_found = true;
            ovel = o_vel;
            ovel_full = o_vel_full;
        }
    }

    bool found() const {
        return did_found;
    }
};

struct CldAgent {
    uint8_t flags;
    BBox bbox;
    vec2 v;
};

void phys_c::cld_solid(GwaCtx& ctx, EntityID self) {
    body_c* body = ctx.em->body(self);
    cld_c* cld = ctx.em->cld(self);
    BBox bbox;

    cld_bundle cb_u(true);
    cld_bundle cb_d(false);
    cld_bundle cb_l(true);
    cld_bundle cb_r(false);

    std::vector<CldAgent> other;
    for (EntityID o : cld->other) {
        CldAgent a;
        a.flags = ctx.em->cld(o)->flags;
        a.bbox = ctx.em->cld(o)->bbox + ctx.em->body(o)->p;
        a.v = ctx.em->has(o, PHYS) ? ctx.em->phys(o)->v : vec2(0, 0);

        other.push_back(a);
    }
    for (const TilePos& tp : ctx.tm->decompress(cld->tilemap_range)) {
        const uint16_t tile_val = ctx.tm->map[tp.y][tp.x];
        CldAgent a;
        vec2 sz = ctx.rm->tileset(ctx.tm->tileset)->sz;
        a.bbox = BBox::from(vec2(tp.x * ctx.tm->sz.x, tp.y * ctx.tm->sz.y), sz);
        a.flags = ctx.rm->tileset(ctx.tm->tileset)->tiles[tile_val].v;
        a.v = vec2(0, 0);

        other.push_back(a);
    }

    // Floor

    bbox = cld->bbox + body->p;
    for (const CldAgent& a : other) {
        if ((a.flags & cld_c::SOLID_F) != cld_c::SOLID_F) {
            continue;
        }

        const BBox obbox = a.bbox;
        const fp6 target_pos = obbox.u;

        const vec2 ov = a.v;
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
            cb_u.send(target_pos, ov.y, ov);
        }
    }

    if (cb_u.found()) {
        cld->solve(bbox.d, cb_u.val, &body->p.y);
        v.y = 0;
        im += cb_u.ovel_full;
    }

    // Ceiling

    bbox = cld->bbox + body->p;
    for (const CldAgent& a : other) {
        if ((a.flags & cld_c::SOLID_C) != cld_c::SOLID_C) {
            continue;
        }

        const BBox obbox = a.bbox;
        const fp6 target_pos = obbox.d;

        const vec2 ov = a.v;
        if (!(bbox).cld_h_exc((obbox).exp(vec2(0, -1)))) {
            continue;
        }

        const fp6 dv = v.y - ov.y;

        if (dv <= 0 && bbox.u >= target_pos && bbox.u + dv <= target_pos) {
            cb_d.send(target_pos, ov.y, ov);
        }
    }

    if (cb_d.found()) {
        cld->solve(bbox.u, cb_d.val, &body->p.y);
        v.y = 0;
        im.y += cb_d.ovel;
    }

    // Rwall

    bbox = cld->bbox + body->p;
    for (const CldAgent& a : other) {
        if ((a.flags & cld_c::SOLID_R) != cld_c::SOLID_R) {
            continue;
        }

        const BBox obbox = a.bbox;
        const fp6 target_pos = obbox.l;

        const vec2 ov = a.v;
        if (!(bbox + (v + im - ov)).cld_v_exc((obbox).exp(vec2(0, 0)))) {
            continue;
        }

        const fp6 dv = v.x + im.x - ov.x;

        if (dv >= 0 && bbox.r <= target_pos && bbox.r + dv >= target_pos) {
            cb_l.send(target_pos, ov.x, ov);
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
    for (const CldAgent& a : other) {
        if ((a.flags & cld_c::SOLID_L) != cld_c::SOLID_L) {
            continue;
        }

        const BBox obbox = a.bbox;
        const fp6 target_pos = obbox.r;

        const vec2 ov = a.v;
        if (!(bbox + (v + im - ov)).cld_v_exc((obbox).exp(vec2(-1, 0)))) {
            continue;
        }

        const fp6 dv = v.x - ov.x;

        if (dv <= 0 && bbox.l >= target_pos && bbox.l + dv <= target_pos) {
            cb_r.send(target_pos, ov.x, ov);
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