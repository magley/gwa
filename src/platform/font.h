#pragma once

#include "resource/fwdecl.h"
#include "util/geometry/vec2.h"
#include "util/string/string2.h"
#include "ctx/fdecl.h"

struct Font {
    TextureH tex;
    vec2 sz;

    void load(GwaCtx& ctx, const string2& serial);
};