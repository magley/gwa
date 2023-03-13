#pragma once

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Color;

struct BBox;
struct vec2;
struct string2;

#include "resource/fwdecl.h"

class Texture {
    void free_mem();
public:
    SDL_Texture* texture = 0;
    int w, h;

    ~Texture();
    Texture();
    Texture(SDL_Renderer* rend, const char* fname);
};

class Renderer {
    SDL_Renderer* rend = 0;
    ResMng* res_mng = 0;
public:
    Renderer(SDL_Renderer* rend, ResMng* res_mng);
    int clear(unsigned char r, unsigned char g, unsigned char b) const;
    void swap_buffers() const;

    void tex(TextureH texture, 
             const vec2& p, 
             float ang, 
             const BBox& src, 
             const BBox& dest,
             const vec2& flip) const;
    void tex(TextureH texture,
             const vec2& p,
             float ang,
             const BBox& src) const;
    void tex(TextureH texture,
             const vec2& p,
             float ang) const;

    void tex_colored(TextureH texture, 
             const vec2& p, 
             float ang, 
             const BBox& src, 
             const BBox& dest,
             const vec2& flip,
             SDL_Color col) const;
    void tex_colored(TextureH texture,
            const vec2& p,
            float ang,
            const BBox& src,
            SDL_Color col) const;

    void tex_sized(TextureH texture,
                   const vec2& p,
                   float ang,
                   const BBox& src,
                   const vec2& scale) const;
    void tex_sized(TextureH texture,
                   const vec2& p,
                   float ang,
                   const vec2& scale) const;

    void rect(const BBox& bbox, SDL_Color col) const;
    void rectf(const BBox& bbox, SDL_Color col) const;

    void line(const vec2& a, const vec2& b, SDL_Color col) const;

    void text(const vec2& pos, FontH font, const string2& s, SDL_Color col) const;
};