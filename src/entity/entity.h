#pragma once

#include <vector>
#include <queue>
#include "component/EVERYTHING.h"
#include <unordered_map>
#include "resource/fwdecl.h"
#include "ctx/fdecl.h"

struct string2;

#include "typedef.h"

enum : ComponentBit {
    PHYS    = 0b00001,
    CLD     = 0b00010,
    ITEM    = 0b00100,
    PLAYER  = 0b01000,
    SPR     = 0b10000,
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
    spr_c    spr;

    string2 save(GwaCtx& ctx) const;
    void load(GwaCtx& ctx, const string2& s);
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
    EntityID get_first(ComponentBit components);

    // Draw a single entity. You don't draw multiple entities yourself because
    // of depth. The main loop takes care of that.
    void rend(EntityID id, GwaCtx& ctx) const;

    body_c*   body(EntityID id) const;
    phys_c*   phys(EntityID id) const;
    cld_c*    cld(EntityID id) const;
    item_c*   item(EntityID id) const;
    player_c* player(EntityID id) const;
    spr_c*    spr(EntityID id) const;

    void set_destroy_flag(Entity* e, uint32_t destroy_flag);
    bool has_destroy_flag(Entity* e, uint32_t destroy_flag);
    void deal_with_entities_marked_for_destruction();
    uint32_t invalidate_refs_to_destroyed_entities();
    void remove_invalid_refs();

    string2 save(GwaCtx& ctx) const;
    void load(GwaCtx& ctx, const string2& s);

};