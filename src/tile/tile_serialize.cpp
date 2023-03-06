#include "tile.h"
#include "unordered_map"
#include "util/string/string2.h"
#include "resource/res_mng.h"
#include "ctx/ctx.h"
#include <cassert>

//=============================================================================
// GENERAL PURPOSE HELPER FUNCS
// TODO: Move this somewhere else. Entity serialization code could use it (and
// other things in the future, too).
//=============================================================================

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

static Declarr decl_arr(const Declmap& map, const string2& key) {
    const auto it = map.find(key);
    const string2 arr_full = it->second[1].trim();
    assert(arr_full[0] == '[');
    assert(arr_full[-1] == ']');
    return arr_full.slice(1, -2).trim()
        .split_unless_between(",", {"{}", "[]"}, false);
}

static Declarr decl_arr_space(const Declmap& map, const string2& key) {
    const auto it = map.find(key);
    const string2 arr_full = it->second[1].trim();
    assert(arr_full[0] == '[');
    assert(arr_full[-1] == ']');

    // HACK: Hardcoded .replace("\n", " "). Not good for general purpose arrays.
    return arr_full.slice(1, -2).trim().replace("\n", " ")
        .split_unless_between(" ", {"{}", "[]"}, false);
}

static vec2 load_vec2(const Declmap& map, const string2& key) {
    auto it = map.find(key);
    vec2 v;
    v.x = it->second[1].to_d();
    v.y = it->second[2].to_d(); 
    return v;
}

static std::vector<vec2> load_vec2_arr_inline(const Declmap& map, const string2& key) {
    auto it = map.find(key);
    std::vector<vec2> res;
    for (int i = 1; i < it->second.size() - 1; i += 2) {
        fp6 x = it->second[i].to_d();
        fp6 y = it->second[i + 1].to_d();
        res.push_back({x, y});
    }
    return res;
}

static string2 save_vec2_arr_inline(const std::vector<vec2>& arr) {
    std::vector<string2> pos_vec_s;
    for (const vec2& v : arr) {
        pos_vec_s.push_back(string2::from((int)v.x));
        pos_vec_s.push_back(string2::from((int)v.y));
    }
    return string2::join(pos_vec_s, " ");
}

static string2 save_vec2(const vec2& v) {
    return string2::from((double)v.x) + " " + string2::from((double)v.y);
}

//=============================================================================
// TILE MAP SAVE/LOAD
//=============================================================================

string2 TileMap::save(GwaCtx& ctx) const {
    string2 s;
    s += "{\n";
    s += "layers [";
    for (const TileMapLayer& layer : layers) {
        s += layer.save(ctx);
        s += ",";
    }
    s += "]\n";
    s += "}\n";
    return s;
}

void TileMap::load(GwaCtx& ctx, const string2& s) {
    const Declmap mp = decl_map(s);

    const Declarr layers_list = decl_arr(mp, "layers");
    for (const string2& layer_decl : layers_list) {
        TileMapLayer layer;
        layer.load(ctx, layer_decl);
        layers.push_back(layer);
    }
}

//=============================================================================
// TILESET SAVE/LOAD
//=============================================================================

void Tileset::load(GwaCtx& ctx, const string2& s) {
    tiles.clear();

    Declmap map = decl_map(s);

    tex = ctx.rm->texture(map.find("tex")->second[1]);
    sz = load_vec2(map, "sz");

    Declarr tiles_arr = decl_arr(map, "tiles");
    for (const auto& tile : tiles_arr) {
        const Declmap tdecl_map = decl_map(tile);
        const int v = tdecl_map.find("v")->second[1].to_l();
        const std::vector<vec2> pos = load_vec2_arr_inline(tdecl_map, "pos");
        tiles.push_back(Tile(v, pos));   
    }
}

string2 Tileset::save(GwaCtx& ctx) const {
    string2 s;
    s += "{\n";

    s += "tex " + ctx.rm->texture_rev(tex) + "\n";
    s += "sz " + save_vec2(sz) + "\n";
    s += "tiles [";
    for (const auto& t : tiles) {
        s += "{";
        s += "v " + string2::from(t.v) + "\n";
        s += "pos " + save_vec2_arr_inline(t.pos) + "\n";
        s += "},";
    }
    s += "]\n";

    s += "}\n";
    return s;
}

//=============================================================================
// TILE MAP LAYER SAVE/LOAD
//=============================================================================

void TileMapLayer::load(GwaCtx& ctx, const string2& layer_decl_str) {
    const Declmap layer_map = decl_map(layer_decl_str);
    
    const vec2 grid_sz = load_vec2(layer_map, "sz");
    const vec2 wh = load_vec2(layer_map, "wh");
    const int d = layer_map.find("depth")->second[1].to_i();
    const Declarr layer_map_decl = decl_arr_space(layer_map, "map");

    depth = d;
    sz = grid_sz;
    tileset = ctx.rm->tileset(layer_map.find("tileset")->second[1]);

    int k = 0;
    for (const string2& s : layer_map_decl) {
        if (k % wh.x == 0) {
            map.push_back({});
            k = 0;
        }

        map[map.size() - 1].push_back(s.to_i());
        k++;
    }
}

string2 TileMapLayer::save(GwaCtx& ctx) const {
    string2 s;
    s += "{\n";
        s += "sz " + save_vec2(sz) + "\n";
        s += "tileset " + ctx.rm->tileset_rev(tileset) + "\n";
        s += "wh " + save_vec2({map[0].size(), map.size()}) + "\n";
        s += "depth " + string2::from(depth) + "\n";
        s += "map [\n";
            {
                const int tiles_per_line = 100;
                int k = 0;
                for (int y = 0; y < map.size(); y++) {
                    for (int x = 0; x < map[y].size(); x++) {
                        k++;
                        s += string2::from(map[y][x]) + " ";
                        if (k == tiles_per_line) {
                            k = 0;
                            s += "\n";
                        }
                    }
                }
            }
        s += "]\n";
    s += "}\n";
    return s;
}