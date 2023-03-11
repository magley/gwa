#pragma once

#include "util/fixed/fp.h"
struct vec2;

struct BBox {
    fp6 u, d, l, r;

    BBox();
    BBox(const BBox& o);

    static BBox from(fp6 u, fp6 l, fp6 d, fp6 r);
    static BBox from(const vec2& pos, const vec2& size);

    bool cld_h_exc(const BBox& other) const;
    bool cld_v_exc(const BBox& other) const;
    bool cld_exc(const BBox& other) const;

    bool cld_h(const BBox& other) const;
    bool cld_v(const BBox& other) const;
    bool cld(const BBox& other) const;

    bool test(const vec2& point) const;
    vec2 size() const;

    BBox operator+(const vec2& v) const;
    BBox& operator+=(const vec2& v);
    BBox exp(const vec2& v) const;
};