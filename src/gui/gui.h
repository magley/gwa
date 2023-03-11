#pragma once

#include <cstdint>
#include <unordered_map>
#include "util/geometry/vec2.h"
#include "util/geometry/bbox.h"
#include "ctx/fdecl.h"

using WidgetH = int16_t;

struct GuiCheckbox {
    WidgetH id;
    BBox extents;

    GuiCheckbox(WidgetH id, BBox extents): id(id), extents(extents) {};
};

struct Gui {
    std::unordered_map<WidgetH, GuiCheckbox> checkbox_map;
    WidgetH counter = 0;
    WidgetH focus = -1;
    vec2 layout = vec2(0, 0);
    GwaCtx* ctx;

    Gui(GwaCtx* ctx): ctx(ctx) {};

    void begin();
    void checkbox(bool* v);

    void draw();
};