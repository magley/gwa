#include "gui.h"
#include "ctx/ctx.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_mouse.h"
#include "platform/font.h"
#include "util/string/string2.h"

void Gui::init_ctx(GwaCtx* ctx) {
    this->ctx = ctx;
}

void Gui::begin() {
    hot_prev = hot;
    active_prev = active;

    hot = -1;
    cnt = 0;

    if (font_small == -1) {
        font_small = ctx->rm->font("../res/font/font_12pt.font");
    }
}

void Gui::end() {
    const Input& input = *ctx->input;
    const Renderer& rend = *ctx->rend;

    if (!input.m_down(SDL_BUTTON_LMASK)) {
        active = -1;
    }

    if (active != -1) {
        input_captured = true;
    } else {
        input_captured = false;
    }
}

bool Gui::hasfocus() {
    return hot_prev != -1 || input_captured;
}

bool Gui::button(const vec2& pos, const string2& s) {
    cnt++;
    const int16_t id = cnt;
    const Input& input = *ctx->input;
    const Renderer& rend = *ctx->rend;
    ResMng& rm = *ctx->rm;

    vec2 p = pos;
    if (!win_stack.empty()) {
        const _WinStackElem& owner = win_stack.top();
        p += map_windows[owner.name].pos;

        if (map_windows[owner.name].expanded == false) {
            return false;
        }
    }

    const vec2 chsz = rm.font(font_small)->cell_size; // TODO: Variable width.

    const BBox bbox = BBox::from(p, vec2(chsz.x * s.size(), chsz.y)).exp(vec2(1, 1));
    if (bbox.test(input.m_pos_raw())) {
        hot = id;

        if (/*active == -1 &&*/ input.m_down(SDL_BUTTON_LMASK)) {
            active = id;
        }
    }

    rend.rectf(bbox, {200, 200, 200, 255});
    rend.rect(bbox, {0, 0, 0, 255});

    if (hot == id) {
        rend.rect(bbox, {255, 255, 255, 255});
    }

    if (active == id) {
            rend.rectf(bbox.exp(vec2(-1, -1)), {64, 64, 64, 255});
            rend.rect(bbox, {128, 128, 128, 255});
    }

    rend.text(p + vec2(active == id, active == id), font_small, s, {0,0,0,255});

    if (hot == id && active == id && !input.m_down(SDL_BUTTON_LMASK)) {
        return true;
    }
    return false;
}

void Gui::checkbox(const vec2& pos, const vec2& sz, bool* b, bool omni) {
    cnt++;
    const int16_t id = cnt;
    const Input& input = *ctx->input;
    const Renderer& rend = *ctx->rend;

    vec2 p = pos;
    if (!win_stack.empty()) {
        const _WinStackElem& owner = win_stack.top();
        p += map_windows[owner.name].pos;

        if (map_windows[owner.name].expanded == false && !omni) {
            return;
        }
    }

    const BBox bbox = BBox::from(p, sz);
    if (bbox.test(input.m_pos_raw())) {
        hot = id;

        if (/*active == -1 && */input.m_down(SDL_BUTTON_LMASK)) {
            active = id;
        }
    }

    rend.rectf(bbox, {200, 200, 200, 255});
    rend.rect(bbox, {0, 0, 0, 255});

    if (hot == id) {
        if (active == id) {
            rend.rectf(bbox.exp(vec2(-1, -1)), {64, 64, 64, 255});
            rend.rect(bbox, {128, 128, 128, 255});
        } else {
            rend.rect(bbox, {255, 255, 255, 255});
        }
    }

    if (*b) {
        rend.rectf(bbox.exp({-2, -2}), {12, 88, 255, 255});
    } else {
        rend.rectf(bbox.exp({-2, -2}), {32, 32, 32, 255});
    }

    if (hot == id && active == id && !input.m_down(SDL_BUTTON_LMASK)) {
        *b ^= true;
    }
}

void Gui::startwin(const string2& name, const vec2& pos, const vec2& size) {
    cnt++;
    const int16_t id = cnt;
    const Input& input = *ctx->input;
    const Renderer& rend = *ctx->rend;

    _WinState state;
    const auto it = map_windows.find(name);

    if (it != map_windows.end()) {
        _WinState state_prev = it->second;
        state.pos = state_prev.pos;
        state.expanded = state_prev.expanded;
    } else {
        state.pos = pos;
        state.expanded = true;
    }

    BBox bbox = BBox::from(state.pos, state.expanded ? size : vec2(size.x, 8));
    BBox bbox_h = BBox::from(state.pos, {size.x, 8});
    if (bbox.test(input.m_pos_raw())) {
        hot = id;

        if (bbox_h.test(input.m_pos_raw())) {
            if (active == -1 && input.m_down(SDL_BUTTON_LMASK)) {
                active = id;
            }
        }
    }

    state.drag_diff = (input.m_pos_raw() - state.pos);
    if (active == id) {          
        const auto it = map_windows.find(name);
        if (it != map_windows.end()) {
            const _WinState state_prev = it->second;

            state.pos = input.m_pos_raw() - state_prev.drag_diff;
            state.drag_diff = state_prev.drag_diff;
        }
    } 

    win_stack.push({cnt, name});
    map_windows[name] = state;

    

    vec2 sz = size;
    if (map_windows[name].expanded == false) {
        sz.y = bbox_h.size().y;
    }

    bbox = BBox::from(state.pos, sz);
    rend.rectf(bbox, {200, 200, 200, 255});
    rend.rectf(BBox::from(state.pos, bbox_h.size()), {0, 0, 255, 255});
    rend.rect(bbox, {0, 0, 0, 255});

    checkbox({0, 0}, {8, 8}, &map_windows[name].expanded, true);
}

void Gui::endwin() {
    win_stack.pop();
}