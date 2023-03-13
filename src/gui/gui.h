#pragma once

#include <cstdint>
#include <unordered_map>
#include "util/geometry/vec2.h"
#include "util/geometry/bbox.h"
#include "ctx/fdecl.h"
#include "resource/fwdecl.h"
#include <stack>
#include "util/string/string2.h"

using GuiId = int16_t;

struct _WinState {
    vec2 pos;
    vec2 drag_diff;
    bool expanded = true;
};

struct _WinStackElem {
    int16_t id;
    string2 name;
};

struct Gui {
    GwaCtx* ctx;
    void init_ctx(GwaCtx* ctx);

    std::unordered_map<string2, _WinState> map_windows;
    std::stack<_WinStackElem> win_stack;
    FontH font_small =  -1;
    int16_t cnt = 0;

    GuiId hot = -1;
    GuiId active = -1;

    // We don't seperate draw calls from GUI logic, so they must come at the
    // very end. To have other parts of the engine (level editor for example)
    // ignore user input when the user focuses on the GUI, we need the previous
    // state because the current state is altered by the time the editor gets
    // to call its update code.

    GuiId hot_prev = -1;
    GuiId active_prev = -1;
    bool input_captured = false;

    void begin();
    void end();
    bool hasfocus();

    bool button(const vec2& pos, const string2& s);
    void checkbox(const vec2& pos, const vec2& sz, bool* b, bool omni = false);
    
    void startwin(const string2& name, const vec2& pos, const vec2& size);
    void endwin();
};