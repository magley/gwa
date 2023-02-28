#include "res_mng.h"
#include "platform/video.h"

ResMng::ResMng(SDL_Renderer* rend): rend(rend) {

}

ResMng::~ResMng() {
    for (auto o : textures) {
        delete o;
    }
}

TextureH ResMng::texture(const char* fname) {
    auto it = res_handles.find(fname);
    if (it == res_handles.end()) {
        return load_texture_(fname);
    } else {
        return it->second;
    }
}

Texture* ResMng::texture(TextureH tex) {
    return textures[tex];
}

TextureH ResMng::load_texture_(const char* fname) {
    Texture* tex = new Texture(rend, fname);

    TextureH id = textures.size();
    textures.push_back(tex);
    res_handles.insert({fname, id});
    return id;
}