#include "vec2.h"

vec2::vec2(fp6 x, fp6 y): x(x), y(y) {

}

vec2::vec2(const vec2& v): x(v.x), y(v.y) {

}

vec2::vec2() {

}

vec2 vec2::abs() const {
    return vec2(x.abs(), y.abs());
}

bool vec2::operator==(const vec2& v) const {
    return x == v.x && y == v.y;
}

bool vec2::operator!=(const vec2& v) const {
    return x == v.x && y == v.y;
}

vec2& vec2::operator=(const vec2& v) {
    x = v.x;
    y = v.y;
    return *this;
}

vec2 vec2::operator+(const vec2& v) const {
    vec2 w = *this;
    w.x += v.x;
    w.y += v.y;
    return w;
}

vec2& vec2::operator+=(const vec2& v) {
    x += v.x;
    y += v.y;
    return *this;
}

vec2 vec2::operator-(const vec2& v) const {
    vec2 w = *this;
    w.x -= v.x;
    w.y -= v.y;
    return w;
}

vec2& vec2::operator-=(const vec2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

vec2 vec2::operator*(const fp6& f) const {
    vec2 w = *this;
    w.x *= f;
    w.y *= f;
    return w;
}

vec2& vec2::operator*=(const fp6& f) {
    x *= f;
    y *= f;
    return *this;
}

vec2 vec2::operator/(const fp6& f) const {
    vec2 w = *this;
    w.x /= f;
    w.y /= f;
    return w;
}

vec2& vec2::operator/=(const fp6& f) {
    x *= f;
    y *= f;
    return *this;
}

vec2 vec2::operator+() const {
    return *this;
}

vec2 vec2::operator-() const {
    vec2 w = *this;
    w.x = -w.x;
    w.y = -w.y;
    return w;
}

vec2 operator*(const fp6& f, const vec2& v) {
    vec2 w = v;
    w.x *= f;
    w.y *= f;
    return w;
}