#include "entity.h"
#include <assert.h>
#include <cstring>

//-----------------------------------------------------------------------------
// Entity
//-----------------------------------------------------------------------------

void Entity::add(int component) { 
    c[component] = true; 
}

void Entity::rem(int component) { 
    c[component] = false; 
}

bool Entity::get(int component) const { 
    return c[component]; 
}

void Entity::reset_component_flags() { 
    memset(c, false, count_COMPONENT); 
}

//-----------------------------------------------------------------------------
// EntityManager
//-----------------------------------------------------------------------------

#define UNDERSCORE(a, b) a ## _ ## b
#define INIT(cname)                                             \
{                                                               \
    e->cname = new UNDERSCORE(cname, c)();                      \
}
#define GET(cname)                                                \
UNDERSCORE(cname, c)* EntityManager::cname(const EntityID& id) {  \
    return entity[id]->cname;                                     \
}

GET(transform);
GET(physics);

void EntityManager::init_components(Entity* e) {
    INIT(transform);
    INIT(physics);
}

EntityID EntityManager::add() {
    Entity* e;

    const EntityID free_id = get_empty_id();
    if (free_id == -1) {
        e = new Entity();
        e->id = entity.size();
        entity.push_back(e);
        init_components(e);
    } else {
        e = entity[free_id];
        e->reset_component_flags();
        e->del = 0;
    }

    return e->id;
}

EntityID EntityManager::get_empty_id() {
    if (empty_ids.empty()) {
        return -1;
    }
    EntityID id = empty_ids.front();
    empty_ids.pop();
    return id;
}

EntityRefID EntityManager::make_ref(const EntityID& e) {
    EntityRef* ref = new EntityRef();

    if (e == -1) {
        ref->entity = e;
        ref->id = ref_cnt++;
        ref->valid = false;
    } else {
        ref->entity = e;
        ref->id = ref_cnt++;
        ref->valid = true;
    }

    refs.push_back(ref);
    return ref->id;
}

EntityRef EntityManager::get_ref(const EntityRefID& refid) {
    EntityRef d;

    for (int i = 0; i < refs.size(); i++) {
        if (refs[i]->id == refid) {
            return *refs[i];
        }
    }

    d.valid = false;
    return d;
}

void EntityManager::cleanup() {
    // "Delete" all entities marked for deletion.
    for (int i = 0; i < entity.size(); i++) {
        if (entity[i]->del == 1) {
            entity[i]->del = 2;
            empty_ids.push(i);
        }
    }

    // Invalidate references whose pointing entities are deleted.
    std::vector<EntityRef*> new_refs;
    for (int i = 0; i < refs.size(); i++) {
        if (refs[i]->entity == -1) {
            refs[i]->valid = false;
            delete refs[i];             
        } else if (entity[refs[i]->entity]->del == 2) {
            refs[i]->valid = false;
            delete refs[i];
        } else {
            new_refs.push_back(refs[i]);
        }
    }

    refs = new_refs;
}

void EntityManager::rem(const EntityID& id) {
    assert(id < entity.size());
    if (!entity[id]->del) {
        entity[id]->del = 1;
    }
}

bool EntityManager::is_rem(const EntityID& id) const {
    assert(id < entity.size());
    return entity[id]->del > 0;
}

void EntityManager::add_c(const EntityID& id, int c) const {
    assert(id < entity.size());
    entity[id]->add(c);
}

void EntityManager::rem_c(const EntityID& id, int c) const {
    assert(id < entity.size());
    entity[id]->rem(c);
}

bool EntityManager::get_c(const EntityID& id, int c) const {
    assert(id < entity.size());
    return entity[id]->get(c);
}


#undef GET
#undef INIT
#undef UNDERSCORE