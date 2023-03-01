#pragma once


#include <unordered_map>
#include <vector>

#include "util/string/string2.h"
#include "fwdecl.h"

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
class ResMng {
public:
    SDL_Renderer* rend;

    ResMng(SDL_Renderer* rend);
    ~ResMng();

    void clean();

    std::unordered_map<string2, TextureH> texture_h;
    std::vector<Texture*> textures;
    TextureH texture(const string2& fname);
    Texture* texture(TextureH tex);
    string2 texture_rev(TextureH tex);

    TextureH load_texture_(const string2& fname);
};