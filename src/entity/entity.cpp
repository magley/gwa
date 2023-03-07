#include "entity.h"
#include <assert.h>
#include <cstring>
#include "ctx/ctx.h"
#include "SDL2/SDL_render.h"

EntityManager::EntityManager() {
}

//-----------------------------------------------------------------------------
// High-level methods. For external use.
//-----------------------------------------------------------------------------

int EntityManager::count() const {
    return entity_count;
}

EntityID EntityManager::create() {
    Entity* e;
    
    if (free_entity_slots.empty()) {
        e = new Entity();
        e->id = entity.size();
        entity.push_back(e);
    } else {
        e = entity[free_entity_slots.front()];
        free_entity_slots.pop();
        set_destroy_flag(e, ALIVE);
    }

    entity_count++;
    return e->id;
}

void EntityManager::destroy(EntityID id) {
    set_destroy_flag(entity[id], TO_DESTROY);
}

bool EntityManager::destroyed(EntityID id) {
    return !has_destroy_flag(entity[id], ALIVE);
}

EntityRefID EntityManager::make_ref(EntityID id) {
    EntityRef* ref = new EntityRef();
    ref->id = entity_ref_sequencer;
    ref->entity = id;
    ref->valid = !destroyed(id);

    refs.insert({ref->id, ref});
    entity_ref_sequencer++;
    return ref->id;
}

EntityRef EntityManager::get_ref(EntityRefID ref) {
    auto it = refs.find(ref);
    if (it == refs.end()) {
        EntityRef bad;
        bad.id = ref;
        bad.entity = ENTITY_NULL;
        bad.valid = false;
        return bad;
    }
    return *(it->second);
}

void EntityManager::add(EntityID id, ComponentBit components) {
    entity[id]->c |= components;
}

void EntityManager::rem(EntityID id, ComponentBit components) {
    entity[id]->c = entity[id]->c & ~components;
}

bool EntityManager::has(EntityID id, ComponentBit components) const {
    return (entity[id]->c & components) == components;
}

std::vector<EntityID> EntityManager::get_all(ComponentBit components) {
    std::vector<EntityID> result;
    for (Entity* e : entity) {
        if (destroyed(e->id)) {
            continue;
        } 
        if ((e->c & components) == components) {
            result.push_back(e->id);
        }
    }
    return result;
}

EntityID EntityManager::get_first(ComponentBit components) {
    for (Entity* e : entity) {
        if (destroyed(e->id)) {
            continue;
        }
        if (has(e->id, components)) {
            return e->id;
        }
    }
    return ENTITY_NULL;
}

void EntityManager::cleanup() {
    deal_with_entities_marked_for_destruction();
    uint32_t invalid_refs = invalidate_refs_to_destroyed_entities();

    if (invalid_refs >= refs.size() / 2 && refs.size() > entity.size()) {
        remove_invalid_refs();
    }
}

void EntityManager::rend(EntityID e, GwaCtx& ctx) const {
    if (has(e, CLD)) {
        body_c* b = body(e);
        cld_c* c = cld(e);

        ctx.rend->rect(c->bbox + (b->p - ctx.cam), {180, 180, 180, 255});
        if (c->other.size() == 0) {
        } else {
            ctx.rend->rectf(c->bbox + (b->p - ctx.cam), {32, 255, 96, 96});
        }
    }

    if (has(e, SPR)) {
        body_c* b = body(e);
        spr_c* s = spr(e);
        TextureH tex = s->tex;

        if (has(e, CLD)) {
            cld_c* c = cld(e);
            ctx.rend->tex_sized(tex, b->p - ctx.cam, 0, c->bbox.size());
        } else {
            ctx.rend->tex(tex, b->p - ctx.cam, 0);
        }
    }
}

//-----------------------------------------------------------------------------
// Component methods.
//-----------------------------------------------------------------------------

body_c* EntityManager::body(EntityID id) const {
    return &entity[id]->body;
}

phys_c* EntityManager::phys(EntityID id) const {
    return &entity[id]->phys;
}

cld_c* EntityManager::cld(EntityID id) const {
    return &entity[id]->cld;
}

item_c* EntityManager::item(EntityID id) const {
    return &entity[id]->item;
}

player_c* EntityManager::player(EntityID id) const {
    return &entity[id]->player;
}

spr_c* EntityManager::spr(EntityID id) const {
    return &entity[id]->spr;
}

//-----------------------------------------------------------------------------
// Low-level methods. For internal use.
//-----------------------------------------------------------------------------

#define RIGHTMOST_0(n) (((~0 >> n) << n))
#define RIGHTMOST_1(n) ~RIGHTMOST_0(n)

void EntityManager::set_destroy_flag(Entity* e, uint32_t destroy_flag) {
    uint32_t f = e->flags;
    f &= RIGHTMOST_0(2);
    f |= destroy_flag;
    e->flags = f;
}

bool EntityManager::has_destroy_flag(Entity* e, uint32_t destroy_flag) {
    uint32_t f = e->flags; 
    f &= RIGHTMOST_1(2);
    return f == destroy_flag;
}

void EntityManager::deal_with_entities_marked_for_destruction() {
    for (uint32_t i = 0; i < entity.size(); i++) {
        if (has_destroy_flag(entity[i], TO_DESTROY)) {
            set_destroy_flag(entity[i], DESTROYED);
            free_entity_slots.push(i);
            entity_count--;
        }
    }
}
    
uint32_t EntityManager::invalidate_refs_to_destroyed_entities() {
    uint32_t invalid_refs = 0;
    for (auto it = refs.begin(); it != refs.end(); it++) {
        EntityID e = it->second->entity;
        if (destroyed(e)) {
            it->second->valid = false;
        }
        if (!it->second->valid) {
            invalid_refs++;
        }
    }
    return invalid_refs;
}

void EntityManager::remove_invalid_refs() {
    std::unordered_map<EntityRefID, EntityRef*> newmap;

    for (auto it = refs.begin(); it != refs.end(); it++) {
        if (it->second->valid) {
            newmap.insert(*it);
        } else {
            delete it->second;
        }
    }

    refs = newmap;
}

#undef RIGHTMOST_1
#undef RIGHTMOST_0