#pragma once

#include "util/fixed/fp.h"

struct vec2 {
    fp6 x, y;

    vec2(fp6 x, fp6 y);
    vec2(const vec2& v);
    vec2();

    vec2 abs() const;
    bool operator==(const vec2& v) const;
    bool operator!=(const vec2& v) const;
    vec2& operator=(const vec2& v);
    vec2 operator+(const vec2& v) const;
    vec2 operator-(const vec2& v) const;
    vec2 operator*(const fp6& v) const;
    vec2 operator/(const fp6& v) const;
    vec2& operator+=(const vec2& v);
    vec2& operator-=(const vec2& v);
    vec2& operator*=(const fp6& v);
    vec2& operator/=(const fp6& v);
    vec2 operator+() const;
    vec2 operator-() const;
};

vec2 operator*(const fp6& f, const vec2& v);