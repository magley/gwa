#include "entity.h"
#include <assert.h>
#include <cstring>

EntityManager::EntityManager() {
    // NOTE: First element is always there. Never used because of ENTITY_NULL.
    create();
}

//-----------------------------------------------------------------------------
// High-level methods. For external use.
//-----------------------------------------------------------------------------

EntityID EntityManager::create() {
    Entity* e;
    
    if (entity_slots.empty()) {
        e = new Entity();
        e->id = entity.size();

        entity.push_back(e);

    } else {
        e = entity[entity_slots.front()];
        entity_slots.pop();
        set_destroy_flag(e, ALIVE);
    }

    return e->id;
}

void EntityManager::destroy(EntityID id) {
    set_destroy_flag(entity[id], TO_DESTROY);
}

bool EntityManager::destroyed(EntityID id) {
    return !has_destroy_flag(entity[id], ALIVE);
}

void EntityManager::add(EntityID id, uint64_t components) {
    entity[id]->c |= components;
}

void EntityManager::rem(EntityID id, uint64_t components) {
    entity[id]->c = entity[id]->c & ~components;
}

bool EntityManager::has(EntityID id, uint64_t components) {
    return (entity[id]->c & components) == components;
}

std::vector<EntityID> EntityManager::get_all(uint64_t components) {
    std::vector<EntityID> result;
    for (Entity* e : entity) {
        if ((e->c & components) == components) {
            result.push_back(e->id);
        }
    }
    return result;
}

void EntityManager::cleanup() {
    for (uint32_t i = 0; i < entity.size(); i++) {
        if (has_destroy_flag(entity[i], TO_DESTROY)) {
            set_destroy_flag(entity[i], DESTROYED);
            entity_slots.push(i);
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

#undef RIGHTMOST_1
#undef RIGHTMOST_0