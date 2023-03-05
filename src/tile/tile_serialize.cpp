#include "tile.h"
#include "unordered_map"
#include "util/string/string2.h"
#include "resource/res_mng.h"
#include "ctx/ctx.h"

static string2 save_tile(const Tile& t);
static void save_sz(string2& s, const TileMap* tm);
static void save_wh(string2& s, const TileMap* tm);
static void save_tileset(string2& s, const TileMap* tm, ResMng& rm);
static void save_map(string2& s, const TileMap* tm);

static void load_tile(const string2& s, Tile* t);
static void load_sz(std::vector<string2>& m, TileMap* tm);
static void load_wh(std::vector<string2>& m, int* w, int* h);
static void load_tileset(std::vector<string2>& m, TileMap* tm, ResMng& rm);
static void load_map(std::vector<string2>& m, TileMap* tm, int w, int h);

string2 TileMap::save(GwaCtx& ctx) const {
    string2 s;
    s += "{\n";

    save_sz(s, this);
    save_wh(s, this);
    save_tileset(s, this, *ctx.rm);
    save_map(s, this);

    s += "},\n";
    return s;
}

void TileMap::load(GwaCtx& ctx, const string2& s) {
    // TODO: One .tile file is a collection of all tilemaps. Right now, we just
    // override data. Later when we have a ctx object, implement support for 
    // multiple tilemaps per level.

    auto entity_parts = s.split_unless_between(",", {"{}"});
    for (const auto& s : entity_parts) {
        if (s.trim().empty()) {
            continue;
        }
        
        map.clear();
        tileset = -1;

        const auto decls = s.slice(1, -2).split_unless_between("\n", {"[]"}, false);
        std::unordered_map<string2, std::vector<string2>> map;

        for (const auto& decl : decls) {
            auto parts = decl.trim().split_unless_between(" ", {"\"\""}, false);
            map.insert({parts[0], parts});
        }

        int w, h;
        load_sz(map.find("sz")->second, this);
        load_wh(map.find("wh")->second, &w, &h);
        load_tileset(map.find("tileset")->second, this, *ctx.rm);
        load_map(map.find("map")->second, this, w, h);
    }
}

void Tileset::load(GwaCtx& ctx, const string2& s) {
    tiles.clear();

    const auto decls = s.slice(1, -2).split_unless_between("\n", {"[]"}, false);
    std::unordered_map<string2, std::vector<string2>> map;
    for (const auto& decl : decls) {
        auto parts = decl.trim().split_unless_between(" ", {"\"\""}, false);
        map.insert({parts[0], parts});
    }

    tex = ctx.rm->texture(map.find("tex")->second[1]);

    auto tileset_vals_str = map.find("tiles")->second;
    for (int i = 1; i < tileset_vals_str.size(); i++) {
        tiles.push_back(tileset_vals_str[i].to_i());
    }

    auto it = map.find("sz");
    sz.x = it->second[1].to_d();
    sz.y = it->second[2].to_d();
}

string2 Tileset::save(GwaCtx& ctx) const {
    string2 s;
    s += "{\n";

    s += "tex " + ctx.rm->texture_rev(tex) + "\n";
    s += "sz " + string2::from((int)sz.x) + " " + string2::from((int)sz.y) + "\n";
    s += "tiles ";
    for (const auto& t : tiles) {
        s += string2::from(t.v) + " ";
    }
    s += "\n";

    s += "}\n";
    return s;
}

//=============================================================================
// Internal helper functions - SAVE
//=============================================================================

static string2 save_tile(const Tile& t) {
    string2 s;
    s += string2::from(t.v);
    return s;
}

static void save_sz(string2& s, const TileMap* tm) {
    s += "sz ";
    string2 szx = string2::from((int)tm->sz.x);
    string2 szy = string2::from((int)tm->sz.y);
    s += string2::join({szx, szy}, " ");
    s += "\n";
}

static void save_wh(string2& s, const TileMap* tm) {
    s += "wh ";
    string2 w = string2::from((int)tm->map[0].size());
    string2 h = string2::from((int)tm->map.size());
    s += string2::join({w, h}, " ");
    s += "\n";
}

static void save_tileset(string2& s, const TileMap* tm, ResMng& rm) {
    s += "tileset ";
    s += rm.tileset_rev(tm->tileset);
    s += "\n";
}

static void save_map(string2& s, const TileMap* tm) {
    s += "map [ \n";

    const int tiles_per_line = 100;
    int k = 0;
    for (int y = 0; y < tm->map.size(); y++) {
        for (int x = 0; x < tm->map[y].size(); x++) {
            k++;
            s += string2::from(tm->map[y][x]) + " ";
      
            if (k == tiles_per_line) {
                k = 0;
                s += "\n";
            }
        }
    }
    
    s += "]\n";
}

//=============================================================================
// Internal helper functions - LOAD
//=============================================================================

static void load_tile(const string2& s, Tile* t) {
    t->v = s.to_i();
}

static void load_sz(std::vector<string2>& m, TileMap* tm) {
    fp6 szx = m[1].to_d();
    fp6 szy = m[2].to_d();
    tm->sz = vec2(szx, szy);
}

static void load_wh(std::vector<string2>& m, int* w, int* h) {
    *w = m[1].to_i();
    *h = m[2].to_i();
}

static void load_tileset(std::vector<string2>& m, TileMap* tm, ResMng& rm) {
    tm->tileset = rm.tileset(m[1]);
}

static void load_map(std::vector<string2>& m, TileMap* tm, int w, int h) {
    int start_index = 2; // [0] = "map", [1] = "["
    int end_index = m.size() - 1; // [-1] = "]"

    int k = 0;
    for (int i = start_index; i < end_index; i++) {
        if (k % w == 0) {
            tm->map.push_back({});
            k = 0;
        }

        tm->map[tm->map.size() - 1].push_back(m[i].to_i());
        k++;
    }
}