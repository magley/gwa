#include "res_mng.h"
#include "platform/video.h"
#include "tile/tile.h"
#include "util/file/futil.h"

ResMng::ResMng(SDL_Renderer* rend): rend(rend) {
}

ResMng::~ResMng() {
    clean();
}

void ResMng::clean() {
    for (auto o : textures) {
        delete o;
    }
    textures.clear();
    texture_h.clear();

    for (auto o : tilesets) {
        delete o;
    }
    tilesets.clear();
    tileset_h.clear();
}

//=============================================================================
// Fetch by filename
//=============================================================================

TextureH ResMng::texture(const string2& fname) {
    auto it = texture_h.find(fname);
    if (it == texture_h.end()) {
        return load_texture_(fname);
    } else {
        return it->second;
    }
}

TilesetH ResMng::tileset(const string2& fname) {
    auto it = tileset_h.find(fname);
    if (it == tileset_h.end()) {
        return load_tileset_(fname);
    } else {
        return it->second;
    }
}

//=============================================================================
// Get actual resource
//=============================================================================

Texture* ResMng::texture(TextureH tex) {
    return textures[tex];
}

Tileset* ResMng::tileset(TilesetH tst) {
    return tilesets[tst];
}

//=============================================================================
// Reverse lookup
//=============================================================================

string2 ResMng::texture_rev(TextureH tex) {
    for (auto it = texture_h.begin(); it != texture_h.end(); it++) {
        if (it->second == tex) {
            return it->first;   
        }
    }
    return "";
}

string2 ResMng::tileset_rev(TilesetH tst) {
    for (auto it = tileset_h.begin(); it != tileset_h.end(); it++) {
        if (it->second == tst) {
            return it->first;   
        }
    }
    return "";
}

//=============================================================================
// Raw load
//=============================================================================

TextureH ResMng::load_texture_(const string2& fname) {
    Texture* tex = new Texture(rend, fname.c_str());

    TextureH id = textures.size();
    textures.push_back(tex);
    texture_h.insert({fname, id});
    return id;
}

TilesetH ResMng::load_tileset_(const string2& fname) {
    Tileset* tst = new Tileset();

    string2 tileset_file_contents = from_file(fname.c_str());
    tst->load(*ctx, tileset_file_contents);

    TilesetH id = tilesets.size();
    tilesets.push_back(tst);
    tileset_h.insert({fname, id});

    return id;
}

