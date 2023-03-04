#include "tile.h"
#include "util/string/string2.h"

string2 save_tile(const Tile& t);
void load_tile(const string2& s, Tile* t);

string2 TileMap::save() const {
    string2 s;
    s += "{\n";

    // sz

    s += string2::from((double)sz.x) + " " + string2::from((double)sz.y);
    s += "\n";

    // w, h

    s += string2::from((int)map[0].size()) + " " + string2::from((int)map.size());
    s += "\n";

    // tileset

    for (int i = 0; i < tileset.size(); i++) {
        s += save_tile(tileset[i]) + " ";
    }
    s += "\n";

    // map

    const int line_length = 100; // in units, not chars.
    int k = 0;
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            s += string2::from(map[y][x]) + " ";

            k++;
            if (k == line_length) {
                k = 0;
                s += "\n";
            }
        }
    }
    s += "\n";

    s += "},\n";
    return s;
}

void TileMap::load(const string2& s) {
    // TODO: One .tile file isn'is a collection of all tilemaps.
    // Right now, we just override data. Later when we have a ctx object,
    // implement support for multiple tilemaps per level.

    auto entity_parts = s.split_unless_between(",", {"{}"});
    for (const auto& s : entity_parts) {
        if (s.trim().empty()) {
            continue;
        }
        
        // See the above to-do. That's where this function would be extracted.

        map.clear();
        tileset.clear();

        const auto decls = s.slice(1, -2).split("\n", false);
        
        // sz

        const auto sz_parts = decls[0].trim().split(" ", false);
        fp6 sz_x = sz_parts[0].to_d();
        fp6 sz_y = sz_parts[1].to_d();
        sz = vec2(sz_x, sz_y);

        // w, h

        const auto wh_parts = decls[1].trim().split(" ", false);
        const int w = wh_parts[0].to_d();
        const int h = wh_parts[1].to_d();

        // tileset

        const auto tileset_vs = decls[2].split(" ", false);
        for (const auto& vs : tileset_vs) {
            tileset.push_back(Tile(vs.to_i()));
        }

        // tilemap

        int k = 0;

        for (int l = 3; l < decls.size(); l++) {
            const auto map_line = decls[l].trim().split(" ", false);
            for (const auto& ml_str : map_line) {
                if (k % w == 0) {
                    map.push_back(std::vector<uint16_t>());
                    k = 0;
                }

                map[map.size() - 1].push_back(ml_str.to_i());
                k++;
            }
        }
    }
}

///////

string2 save_tile(const Tile& t) {
    string2 s;
    s += string2::from(t.v);
    return s;
}

void load_tile(const string2& s, Tile* t) {
    t->v = s.to_i();
}