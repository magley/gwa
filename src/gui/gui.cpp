#include "gui.h"
#include "ctx/ctx.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_mouse.h"
#include "platform/font.h"

void Gui::init_ctx(GwaCtx* ctx) {
    this->ctx = ctx;
}

void Gui::begin() {
    hot_prev = hot;
    active_prev = active;

    hot = -1;
    cnt = 0;

    if (font_small == -1) {
        font_small = ctx->rm->font("../res/font_small.font");
    }
}

void Gui::end() {
    const Input& input = *ctx->input;
    const Renderer& rend = *ctx->rend;

    if (!input.m_down(SDL_BUTTON_LMASK)) {
        active = -1;
    }
}

bool Gui::hasfocus() {
    return hot_prev != -1;
}

bool Gui::button(const vec2& pos, const string2& s) {
    cnt++;
    const int16_t id = cnt;
    const Input& input = *ctx->input;
    const Renderer& rend = *ctx->rend;
    ResMng& rm = *ctx->rm;

    const vec2 chsz = rm.font(font_small)->sz;

    const BBox bbox = BBox::from(pos, vec2(chsz.x * s.size(), chsz.y)).exp(vec2(1, 1));
    if (bbox.test(input.m_pos())) {
        hot = id;

        if (active == -1 && input.m_down(SDL_BUTTON_LMASK)) {
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


    rend.text(pos, font_small, s);

    if (hot == id && active == id && !input.m_down(SDL_BUTTON_LMASK)) {
        return true;
    }
    return false;
}

void Gui::checkbox(const vec2& pos, bool* b) {
    cnt++;
    const int16_t id = cnt;
    const Input& input = *ctx->input;
    const Renderer& rend = *ctx->rend;

    const BBox bbox = BBox::from(pos, vec2(12, 12));
    if (bbox.test(input.m_pos())) {
        hot = id;

        if (active == -1 && input.m_down(SDL_BUTTON_LMASK)) {
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
        rend.rectf(bbox.exp({-2, -2}), {64, 200, 64, 255});
    } else {
        rend.rectf(bbox.exp({-2, -2}), {200, 64, 64, 255});
    }

    if (hot == id && active == id && !input.m_down(SDL_BUTTON_LMASK)) {
        *b ^= true;
    }
}