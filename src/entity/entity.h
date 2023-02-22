#pragma once

#include <vector>
#include <queue>
#include "component/EVERYTHING.h"

#define UNDERSCORE(a, b) a ## _ ## b
#define EM_GET(cname) UNDERSCORE(cname, c)* cname(const EntityID& id);
#define E_DECL(cname) UNDERSCORE(cname, c)* cname;

enum {
    TRANSFORM,
    PHYS,
    count_COMPONENT,
};

using EntityID = uint32_t;
using EntityRefID = uint32_t;

struct EntityRef {
    EntityRefID id;
    EntityID entity;
    bool valid;
};

struct Entity {
    E_DECL(transform);
    E_DECL(physics);

    EntityID id;
    int del = 0;
    bool c[count_COMPONENT] = {false};

    void add(int component);
    void rem(int component);
    bool get(int component) const;
    void reset_component_flags();
};

class EntityManager {
    std::queue<EntityID> empty_ids;
    std::vector<Entity*> entity;
    std::vector<EntityRef*> refs;
    EntityRefID ref_cnt = 0; // EntityRef objects don't share slots in the array
    // so their ID is not an array index. ref_cnt is an ID generator.
public:
    EntityID add();
    EntityRefID make_ref(const EntityID& e);
    EntityRef get_ref(const EntityRefID& refid);
    void cleanup();

    void rem(const EntityID& id);
    bool is_rem(const EntityID& id) const;
    void add_c(const EntityID& id, int c) const;
    void rem_c(const EntityID& id, int c) const;
    bool get_c(const EntityID& id, int c) const;

    EM_GET(transform);
    EM_GET(physics);
    EntityID get_empty_id();

    int count() const { return entity.size(); }
private:
    void init_components(Entity* e);
};

#undef GET
#undef UNDERSCORE