#include "player.h"
#include "entity/entity.h"
#include "platform/input.h"
#include "SDL2/SDL_keyboard.h"
#include "ctx/ctx.h"

void player_c::collect_items(GwaCtx& ctx, EntityID self) {
    if (!ctx.em->has(self, CLD)) {
        return;
    }

    cld_c* cld = ctx.em->cld(self);

    for (EntityID& o : cld->other) {
        if (!ctx.em->has(o, ITEM | CLD)) {
            continue;
        }

        item_c* oitem = ctx.em->item(o);

        const bool true_collision = cld->collision_excl(ctx, self, o);

        if (true_collision) {
            oitem->collect();
        }
    }
}

void player_c::move(GwaCtx& ctx, EntityID self) {
    if (!ctx.em->has(self, PHYS)) {
        return;
    }

    body_c* body = ctx.em->body(self);
    phys_c* phys = ctx.em->phys(self);

    fp6 ix = ctx.input->down(SDL_SCANCODE_RIGHT) - ctx.input->down(SDL_SCANCODE_LEFT);
    fp6 iy = ctx.input->down(SDL_SCANCODE_DOWN) - ctx.input->down(SDL_SCANCODE_UP);
    fp6 run = ctx.input->down(SDL_SCANCODE_X);

    fp6 spd = run ? 2.5 : 1;

    phys->v = vec2(ix, iy) * spd;
}