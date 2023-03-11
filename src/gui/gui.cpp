#include "gui.h"
#include "ctx/ctx.h"
#include "SDL2/SDL_video.h"

void Gui::begin() {
    counter = 0;
    focus = -1;
    layout = vec2(0, 0);
    checkbox_map.clear();
}

void Gui::checkbox(bool* v) {
    layout.y += 24;
    const BBox bbox = BBox::from(layout, vec2(16, 16));
    checkbox_map.insert({counter, GuiCheckbox(counter, bbox)});
    counter++;

    const vec2 mouse_pos = ctx->input->m_pos_raw();
    if (bbox.test(mouse_pos)) {
        printf("Hi");
    }
}

void Gui::draw() {
    for (auto& it : checkbox_map) {
        ctx->rend->rectf(it.second.extents, {95, 95, 95, 255});
        ctx->rend->rect(it.second.extents, {255, 255, 255, 255});
    }
}