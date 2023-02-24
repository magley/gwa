#pragma once

#include <vector>
#include <queue>
#include "component/EVERYTHING.h"
#include <unordered_map>

using EntityID = uint32_t;
using EntityRefID = uint32_t;
using ComponentBit = uint32_t;
#define ENTITY_NULL 0

enum : ComponentBit {
    PHYS = 0b0001,
    CLD  = 0b0010,
};

enum : uint32_t {
    ALIVE = 0,
    TO_DESTROY = 1,
    DESTROYED = 2,
};

struct Entity {
    EntityID id;
    ComponentBit c;
    uint32_t flags;

    body_c body; // Always present.
    phys_c phys;
    cld_c  cld;
};

struct EntityRef {
    EntityRefID id;
    EntityID entity;
    bool valid;
};

struct EntityManager {
    std::vector<Entity*> entity;
    std::unordered_map<EntityRefID, EntityRef*> refs;
    std::queue<uint32_t> free_entity_slots;
    int32_t entity_count = 0; // Controlled redundancy.
    EntityRefID entity_ref_sequencer = 0;

    EntityManager();
    EntityID create();
    void destroy(EntityID id);
    bool destroyed(EntityID id);
    EntityRefID make_ref(EntityID id);
    EntityRef get_ref(EntityRefID ref);
    void cleanup();
    int count() const;
    void add(EntityID id, ComponentBit components);
    void rem(EntityID id, ComponentBit components);
    bool has(EntityID id, ComponentBit components);
    std::vector<EntityID> get_all(ComponentBit components);

    body_c* body(EntityID id) const;
    phys_c* phys(EntityID id) const;
    cld_c*  cld(EntityID id) const;

    void set_destroy_flag(Entity* e, uint32_t destroy_flag);
    bool has_destroy_flag(Entity* e, uint32_t destroy_flag);
    void deal_with_entities_marked_for_destruction();
    uint32_t invalidate_refs_to_destroyed_entities();
    void remove_invalid_refs();
};