#include "res_mng.h"
#include "platform/video.h"



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
}

TextureH ResMng::texture(const string2& fname) {
    auto it = texture_h.find(fname);
    if (it == texture_h.end()) {
        return load_texture_(fname);
    } else {
        return it->second;
    }
}

Texture* ResMng::texture(TextureH tex) {
    return textures[tex];
}

string2 ResMng::texture_rev(TextureH tex) {
    for (auto it = texture_h.begin(); it != texture_h.end(); it++) {
        if (it->second == tex) {
            return it->first;   
        }
    }
    return "";
}

TextureH ResMng::load_texture_(const string2& fname) {
    Texture* tex = new Texture(rend, fname.c_str());

    TextureH id = textures.size();
    textures.push_back(tex);
    texture_h.insert({fname, id});
    return id;
}
