#include "video.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

//-----------------------------------------------------------------------------
// Texture
//-----------------------------------------------------------------------------

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
// Renderer
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

void Renderer::draw(const Texture& tex, float x, float y, float ang) const {
    SDL_FRect dest;
    dest.x = x;
    dest.y = y;
    dest.w = tex.w;
    dest.h = tex.h;
    SDL_RenderCopyExF(rend, tex.texture, NULL, &dest, ang, NULL, SDL_FLIP_NONE);
}

void Renderer::draw_ext(
    const Texture& tex, 
    float x, 
    float y, 
    float ang,
    bool flipx, 
    bool flipy, 
    int sx, 
    int sy, 
    int sw, 
    int sh) const 
{
    if (sw < 0) {
        sw = tex.w;
    }
    if (sh < 0) {
        sh = tex.h;
    }
    SDL_Rect src_rect = {sx, sy, sw, sh};
    SDL_FRect dest_rect = {(float)x, (float)y, (float)sw, (float)sh};
    int flip = SDL_FLIP_NONE;
    if (flipx) {
        flip |= SDL_FLIP_HORIZONTAL;
    }

    if (flipy) {
        flip |= SDL_FLIP_VERTICAL;
    }

    SDL_RenderCopyExF(rend, tex.texture, &src_rect, &dest_rect, ang, NULL, 
                      (SDL_RendererFlip )flip);
}