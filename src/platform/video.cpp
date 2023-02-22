#include "video.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

Texture::Texture() {
}

Texture::Texture(SDL_Renderer* rend, const char* fname) {
    if (texture != 0) {
        free_mem();
    }

    texture = IMG_LoadTexture(rend, fname);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Texture::~Texture() {
    free_mem();
}

void Texture::free_mem() {
    SDL_DestroyTexture(texture);
    texture = 0;
}

//-----------------------------------------------------------------------------

Renderer::Renderer(SDL_Renderer* rend): rend(rend) {

}

int Renderer::clear(unsigned char r, unsigned char g, unsigned char b) const {
    int result;
    result = SDL_SetRenderDrawColor(rend, r, g, b, 255);
    if (result != 0) {
        return result;
    }
    result = SDL_RenderClear(rend);
    return result;
}

void Renderer::flip() const {
    SDL_RenderPresent(rend);
}

void Renderer::draw(const Texture& tex, float x, float y) const {
    SDL_FRect dest;
    dest.x = x;
    dest.y = y;
    dest.w = tex.w;
    dest.h = tex.h;
    SDL_RenderCopyF(rend, tex.texture, NULL, &dest);
}

