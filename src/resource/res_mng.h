#pragma once


#include <unordered_map>
#include <vector>

#include "util/string/string2.h"
#include "fwdecl.h"
#include "tile/fdecl.h"
#include "ctx/fdecl.h"

struct Texture;
struct SDL_Renderer;


// Adding a new resource type:
// Declare vector storing the resources.
// Declare methods for getting a handle and the actual resource.
// Declare method for direct loading
// Define methods -- don't forget to put the handle in the map.
// Free resources in ResMng::clean().

// Resource Manager.
// Use handles except in places where you need the actual resource.
// The indirection [fname -> handle -> resource] forces you to rethink whether
// you actually need the resource object or just a handle to the object.
// Don't forget to call init_ctx() before calling any of its methods.
class ResMng {
public:
    GwaCtx* ctx = 0;
    SDL_Renderer* rend;

    ResMng(SDL_Renderer* rend);
    ~ResMng();

    void init_ctx(GwaCtx* ctx) { this->ctx = ctx; }
    void clean();

    std::unordered_map<string2, TextureH> texture_h;
    std::vector<Texture*> textures;
    TextureH texture(const string2& fname);
    Texture* texture(TextureH tex);
    string2 texture_rev(TextureH tex);
    TextureH load_texture_(const string2& fname);

    std::unordered_map<string2, TilesetH> tileset_h;
    std::vector<Tileset*> tilesets;
    TilesetH tileset(const string2& fname);
    Tileset* tileset(TilesetH tst);
    string2 tileset_rev(TilesetH tst);
    TilesetH load_tileset_(const string2& fname);
};