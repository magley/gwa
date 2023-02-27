#include "entity.h"
#include "util/string/string2.h"
#include "stdio.h"
#include "unordered_map"

static void body_from(std::vector<string2>& m, Entity* e);
static void phys_from(std::vector<string2>& m, Entity* e);
static void cld_from(std::vector<string2>& m, Entity* e);
static void item_from(std::vector<string2>& m, Entity* e);
static void player_from(std::vector<string2>& m, Entity* e);

static void body_to(string2& s, const Entity* e);
static void phys_to(string2& s, const Entity* e);
static void cld_to(string2& s, const Entity* e);
static void item_to(string2& s, const Entity* e);
static void player_to(string2& s, const Entity* e);


string2 EntityManager::save() const {
    string2 s;
    for (const auto& e : entity) {
        s += e->save(*this);
    }
    return s;
}

void EntityManager::load(const string2& s) {
    entity.clear();

    auto entity_parts = s.split_unless_between(",", {"{}"}, false);
    for (const auto& s : entity_parts) {
        EntityID e_id = create();
        Entity* e = entity[e_id];
        e->load(*this, s);
    }
}

string2 Entity::save(const EntityManager& em) const {
    string2 s;
    s += "{\n";

    body_to(s, this);

    if (em.has(id, PHYS)) {
        phys_to(s, this);
    }
    if (em.has(id, CLD)) {
        cld_to(s, this);
    }
    if (em.has(id, ITEM)) {
        item_to(s, this);
    }
    if (em.has(id, PLAYER)) {
        player_to(s, this);
    }

    s += "},\n";
    return s;
}

void Entity::load(EntityManager& em, const string2& s) {
    auto decls = s.slice(1, -2).split("\n", false);
    std::unordered_map<string2, std::vector<string2>> map;
    for (const auto& decl : decls) {
        auto parts = decl.trim().split_unless_between(" ", {"\"\""}, false);
        map.insert({parts[0], parts});
    }
    std::unordered_map<string2, std::vector<string2>>::iterator it;

    if ((it = map.find("body")), it != map.end()) {
        body_from(it->second, this);
    }
    if ((it = map.find("phys")), it != map.end()) {
        phys_from(it->second, this);
    }
    if ((it = map.find("cld")), it != map.end()) {
        cld_from(it->second, this);
    }
    if ((it = map.find("item")), it != map.end()) {
        item_from(it->second, this);
    }
    if ((it = map.find("player")), it != map.end()) {
        player_from(it->second, this);
    }
}

//=============================================================================
// Internal helper functions - FROM
//=============================================================================

static void body_from(std::vector<string2>& m, Entity* e) {
    fp6 px = m[1].to_d();
    fp6 py = m[2].to_d();
    fp6 ang = m[3].to_d();
    e->body.p = vec2(px, py);
    e->body.ang = ang;
}

static void phys_from(std::vector<string2>& m, Entity* e) {
    fp6 vx = m[1].to_d();
    fp6 vy = m[2].to_d();
    fp6 rot = m[3].to_d();
    uint8_t flags = m[4].to_d();
    e->c |= PHYS;
    e->phys.v = vec2(vx, vy);
    e->phys.angvel = rot;
    e->phys.flags = flags;
}

static void cld_from(std::vector<string2>& m, Entity* e) {
    fp6 u = m[1].to_d();
    fp6 l = m[2].to_d();
    fp6 d = m[3].to_d();
    fp6 r = m[4].to_d();
    uint8_t flags = m[5].to_d();
    e->c |= CLD;
    e->cld.bbox = BBox::from(u, l, d, r);
    e->cld.flags = flags;
}

static void item_from(std::vector<string2>& m, Entity* e) {
    e->c |= ITEM;
}

static void player_from(std::vector<string2>& m, Entity* e) {
    e->c |= PLAYER;
}

//=============================================================================
// Internal helper functions - TO
//=============================================================================

static void body_to(string2& s, const Entity* e) {
    s += "body ";
    string2 px = string2::from((float)e->body.p.x);
    string2 py = string2::from((float)e->body.p.y);
    string2 ang = string2::from((float)e->body.ang);
    s += string2::join({px, py, ang}, " ");
    s += "\n";
}

static void phys_to(string2& s, const Entity* e) {
    s += "phys ";
    string2 vx = string2::from((float)e->phys.v.x);
    string2 vy = string2::from((float)e->phys.v.y);
    string2 rot = string2::from((float)e->phys.angvel);
    string2 flags = string2::from((float)e->phys.flags);
    s += string2::join({vx, vy, rot, flags}, " ");
    s += "\n";
}

static void cld_to(string2& s, const Entity* e) {
    s += "cld ";
    string2 u = string2::from((float)e->cld.bbox.u);
    string2 l = string2::from((float)e->cld.bbox.l);
    string2 d = string2::from((float)e->cld.bbox.d);
    string2 r = string2::from((float)e->cld.bbox.r);
    string2 flags = string2::from((float)e->cld.flags);
    s += string2::join({u, l, d, r, flags}, " ");
    s += "\n";
}

static void item_to(string2& s, const Entity* e) {
    s += "item ";
    s += "\n";
}

static void player_to(string2& s, const Entity* e) {
    s += "player ";
    s += "\n";
}