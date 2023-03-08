#include "font.h"
#include "ctx/ctx.h"
#include <cassert>

// TODO [tile_serialize.cpp] - Copy-pasting the same code.
using Declarr = std::vector<string2>;
using Declmap = std::unordered_map<string2, Declarr>;

static Declmap decl_map(const string2& s) {
    string2 ss = s.trim();
    assert(ss[0] == '{');
    assert(ss[-1] == '}');
    ss = ss.slice(1, -2).trim();

    const auto decls = ss.split_unless_between("\n", {"[]"}, false);
    Declmap map;
    for (const auto& decl : decls) {
        auto p = decl.trim().split_unless_between(" ", {"\"\"", "[]"}, false);
        map.insert({p[0], p});
    }

    return map;
}

static vec2 load_vec2(const Declmap& map, const string2& key) {
    auto it = map.find(key);
    vec2 v;
    v.x = it->second[1].to_d();
    v.y = it->second[2].to_d(); 
    return v;
}

//////

void Font::load(GwaCtx& ctx, const string2& serial) {
    Declmap map = decl_map(serial);

    string2 tex_fname = map.find("tex")->second[1];
    tex = ctx.rm->texture(tex_fname);
    sz = load_vec2(map, "sz");
}