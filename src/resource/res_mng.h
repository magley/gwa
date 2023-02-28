#pragma once


#include <unordered_map>
#include <vector>

#include "fwdecl.h"

struct Texture;
struct SDL_Renderer;

// Adding a new resource:
// Declare vector storing the resource.
// Declare methods for getting a handle and the actual resource.
// Declare method for direct loading
// Define methods -- don't forget to put the handle in the map.
// Free resources in ResMng::~ResMng().

// Resource Manager.
// Use handles except in places where you need the actual resource.
// The indirection [fname -> handle -> resource] forces you to rethink whether
// you actually need the resource object or just a handle to the object.
class ResMng {
public:
    std::unordered_map<const char*, uint32_t> res_handles;
    SDL_Renderer* rend;

    ResMng(SDL_Renderer* rend);
    ~ResMng();

    std::vector<Texture*> textures;
    TextureH texture(const char* fname);
    Texture* texture(TextureH tex);

    TextureH load_texture_(const char* fname);
};