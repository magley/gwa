#pragma once

#include "util/geometry/bbox.h"
#include <vector>
#include "entity/typedef.h"
#include "tile/fdecl.h"
#include "ctx/fdecl.h"

struct cld_c {
    BBox bbox;

    // Other entities collided against. They have cld_c. Vector may contain
    // false-positives because of how build_other() works, so perform additional
    // collision checks when constructing collision responses.
    std::vector<EntityID> other;
    std::vector<BBoxDiscrete> tilemap_range; // i-th elem = i-th tilemap layer
    enum : uint8_t {
        NONE = 0b0000,
        SOLID_F = 0b0001, // floor
        SOLID_C = 0b0010, // ceiling
        SOLID_L = 0b0100, // left wall (my left)
        SOLID_R = 0b1000, // right wall (my right)
        SOLID = 0b1111, // shorthand for SOLID_F | SOLID_C | SOLID_L | SOLID_R
    };
    uint8_t flags;

    // Use this instead of bbox.cld() because this takes position into account.
    // Will not check if other has a cld.
    // See also collision_excl().
    bool collision(GwaCtx& ctx, EntityID self, EntityID other);

    // Exclusive version of collision() where edge intersection does not pass.
    // Will not check if other has a cld.
    // See also collision().
    bool collision_excl(GwaCtx& ctx, EntityID self, EntityID other);

    void solve(fp6 curr, fp6 desired, fp6* in) const;

    // Find candidates for collision. Bounding boxes are expanded for precision.
    void build_other(GwaCtx& ctx, EntityID self);

    void build_tilemap_range(GwaCtx& ctx, EntityID self);
};