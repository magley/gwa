#include "entity.h"
#include "util/string/string2.h"
#include "stdio.h"
#include "unordered_map"

string2 EntityManager::save() const {
    string2 s;
    return s;
}

void EntityManager::load(const string2& s) {
    entity.clear();
    // TODO: Instead of using a blank entity to shift all IDs by one, just have
    // create() add +1, because this entity.clear() will mess things up.

    auto entity_parts = s.split_unless_between(",", {"{}"}, false);
    for (const auto& s : entity_parts) {
        EntityID e_id = create();
        Entity* e = entity[e_id];
        e->load(s);
    }
}

string2 Entity::save() const {
    string2 s;
    return s;
}

void Entity::load(const string2& s) {
    auto decls = s.slice(1, -2).split("\n", false);
    std::unordered_map<string2, std::vector<string2>> map;
    for (const auto& decl : decls) {
        auto parts = decl.trim().split_unless_between(" ", {"\"\""}, false);
        map.insert({parts[0], parts});
    }
    std::unordered_map<string2, std::vector<string2>>::iterator it;

    if ((it = map.find("body")), it != map.end()) {
        fp6 px = it->second[1].to_d();
        fp6 py = it->second[2].to_d();
        fp6 ang = it->second[3].to_d();
        body.p = vec2(px, py);
        body.ang = ang;
    }
    if ((it = map.find("phys")), it != map.end()) {
        c |= PHYS;
        fp6 vx = it->second[1].to_d();
        fp6 vy = it->second[2].to_d();
        fp6 rot = it->second[3].to_d();
        uint8_t flags = it->second[4].to_d();
        phys.v = vec2(vx, vy);
        phys.angvel = rot;
        phys.flags = flags;
    }
    if ((it = map.find("cld")), it != map.end()) {
        c |= CLD;
        fp6 u = it->second[1].to_d();
        fp6 l = it->second[2].to_d();
        fp6 d = it->second[3].to_d();
        fp6 r = it->second[4].to_d();
        uint8_t flags = it->second[5].to_d();
        cld.bbox = BBox::from(u, l, d, r);
        cld.flags = flags;
    }
}