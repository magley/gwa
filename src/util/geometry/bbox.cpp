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

bool BBox::cld_h(const BBox& other) const {
    return (l >= other.l && l <= other.r)
    || (r >= other.l && r <= other.r);
}

bool BBox::cld_v(const BBox& other) const {
    return (u >= other.u && u <= other.d)
    || (d >= other.u && d <= other.d);
}

bool BBox::cld(const BBox& other) const {
    return cld_h(other) && cld_v(other);
}

vec2 BBox::size() const {
    return vec2(r - l, d - u);
}    

BBox BBox::operator+(const vec2& v) const {
    BBox bbox = *this;
    bbox.l -= v.x;
    bbox.r += v.x;
    bbox.u -= v.y;
    bbox.d += v.y;
    return bbox;
}
    
BBox& BBox::operator+=(const vec2& v) {
    l -= v.x;
    r += v.x;
    u -= v.y;
    d += v.y;
    return *this;
}