#include "bbox.h"
#include "vec2.h"

BBox::BBox() {

}

BBox::BBox(const BBox& o) {
    l = o.l;
    r = o.r;
    u = o.u;
    d = o.d;
}

BBox BBox::from(fp6 u_, fp6 l_, fp6 d_, fp6 r_) {
    BBox b;
    b.u = u_;
    b.l = l_;
    b.d = d_;
    b.r = r_;
    return b;
}

BBox BBox::from(const vec2& pos, const vec2& size) {
    BBox b;
    b.u = pos.y;
    b.l = pos.x;
    b.d = b.u + size.y;
    b.r = b.l + size.x;
    return b;
}

bool BBox::cld_h_exc(const BBox& other) const {
    return (l < other.r && r > other.l);
}

bool BBox::cld_v_exc(const BBox& other) const {
    return (u < other.d && d > other.u);
}

bool BBox::cld_exc(const BBox& other) const {
    return cld_h_exc(other) && cld_v_exc(other);
}

bool BBox::cld_h(const BBox& other) const {
    return (l <= other.r && r >= other.l);
}

bool BBox::cld_v(const BBox& other) const {
    return (u <= other.d && d >= other.u);
}

bool BBox::cld(const BBox& other) const {
    return cld_h(other) && cld_v(other);
}

bool BBox::test(const vec2& point) const {
    return (point.x < r && point.x > l && point.y < d && point.y > u);
}

vec2 BBox::size() const {
    return vec2(r - l, d - u);
}

BBox BBox::operator+(const vec2& v) const {
    BBox bbox = *this;
    bbox.l += v.x;
    bbox.r += v.x;
    bbox.u += v.y;
    bbox.d += v.y;
    return bbox;
}

BBox& BBox::operator+=(const vec2& v) {
    l += v.x;
    r += v.x;
    u += v.y;
    d += v.y;
    return *this;
}

BBox BBox::exp(const vec2& v) const {
    BBox bbox = *this;
    bbox.l -= v.x;
    bbox.r += v.x;
    bbox.u -= v.y;
    bbox.d += v.y;
    return bbox;
}