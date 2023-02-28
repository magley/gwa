#include "player.h"
#include "entity/entity.h"
#include "platform/input.h"
#include "SDL2/SDL_keyboard.h"

void player_c::collect_items(EntityManager& em, EntityID self) {
    if (!em.has(self, CLD)) {
        return;
    }

    cld_c* cld = em.cld(self);

    for (EntityID& o : cld->other) {
        if (!em.has(o, ITEM | CLD)) {
            continue;
        }

        item_c* oitem = em.item(o);

        const bool true_collision = cld->collision_excl(em, self, o);

        if (true_collision) {
            oitem->collect();
        }
    }
}

void player_c::move(EntityManager& em, EntityID self, const Input& input) {
    if (!em.has(self, PHYS)) {
        return;
    }

    body_c* body = em.body(self);
    phys_c* phys = em.phys(self);

    fp6 ix = input.down(SDL_SCANCODE_RIGHT) - input.down(SDL_SCANCODE_LEFT);
    fp6 iy = input.down(SDL_SCANCODE_DOWN) - input.down(SDL_SCANCODE_UP);
    fp6 run = input.down(SDL_SCANCODE_X);

    fp6 spd = run ? 2.5 : 1;

    phys->v = vec2(ix, iy) * spd;
}