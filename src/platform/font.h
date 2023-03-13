#pragma once

#include "resource/fwdecl.h"
#include "util/geometry/vec2.h"
#include "util/string/string2.h"
#include "ctx/fdecl.h"

#include <cstdint>

// struct Font {
//     TextureH tex;
//     vec2 sz;

//     void load(GwaCtx& ctx, const string2& serial);
// };

struct Font {
    TextureH tex;
    vec2 cell_size;
    std::vector<int8_t> base_width;

    void load(GwaCtx& ctx, const string2& serial);
    void load_csv_data(const string2& csv_contents);
};