#pragma once

#include <vector>
#include <queue>
#include "component/EVERYTHING.h"
#include <unordered_map>

struct string2;

#include "typedef.h"

enum : ComponentBit {
    PHYS    = 0b0001,
    CLD     = 0b0010,
    ITEM    = 0b0100,
    PLAYER  = 0b1000,
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

    body_c   body; // Always present.
    phys_c   phys;
    cld_c    cld;
    item_c   item;
    player_c player;

    string2 save(const EntityManager& em) const;
    void load(EntityManager& em, const string2& s);
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
    bool has(EntityID id, ComponentBit components) const;
    std::vector<EntityID> get_all(ComponentBit components);

    body_c*   body(EntityID id) const;
    phys_c*   phys(EntityID id) const;
    cld_c*    cld(EntityID id) const;
    item_c*   item(EntityID id) const;
    player_c* player(EntityID id) const;

    void set_destroy_flag(Entity* e, uint32_t destroy_flag);
    bool has_destroy_flag(Entity* e, uint32_t destroy_flag);
    void deal_with_entities_marked_for_destruction();
    uint32_t invalidate_refs_to_destroyed_entities();
    void remove_invalid_refs();

    string2 save() const;
    void load(const string2& s);
};