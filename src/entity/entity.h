#pragma once

#include <vector>
#include <queue>
#include "component/EVERYTHING.h"

using EntityID = uint32_t;
#define ENTITY_NULL 0

enum : uint64_t {
    PHYS = 1
};

enum : uint32_t {
    ALIVE = 0,
    TO_DESTROY = 1,
    DESTROYED = 2,
};

struct Entity {
    EntityID id;
    uint64_t c;
    uint32_t flags;

    body_c body; // Always present.
    phys_c phys;
};

struct EntityManager {
    std::queue<uint32_t> entity_slots;
    std::vector<Entity*> entity;

    EntityManager();
    EntityID create();
    void destroy(EntityID id);
    bool destroyed(EntityID id);
    void add(EntityID id, uint64_t components);
    void rem(EntityID id, uint64_t components);
    bool has(EntityID id, uint64_t components);
    std::vector<EntityID> get_all(uint64_t components);
    void cleanup();

    body_c* body(EntityID id) const;
    phys_c* phys(EntityID id) const;

    void set_destroy_flag(Entity* e, uint32_t destroy_flag);
    bool has_destroy_flag(Entity* e, uint32_t destroy_flag);
};