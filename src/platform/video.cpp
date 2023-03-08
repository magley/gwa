#include "video.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#include "util/geometry/bbox.h"
#include "util/geometry/vec2.h"

#include "resource/res_mng.h"
#include "util/string/string2.h"
#include "font.h"

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

Renderer::Renderer(SDL_Renderer* rend, ResMng* rm): rend(rend), res_mng(rm) {
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

void Renderer::swap_buffers() const {
    SDL_RenderPresent(rend);
}

//=============================================================================
// New render API.
//=============================================================================

void Renderer::tex(TextureH texture, 
                   const vec2& p, 
                   float ang, 
                   const BBox& src, 
                   const BBox& dest,
                   const vec2& flip) const
{
    Texture* t = res_mng->texture(texture);

    const SDL_Rect s = {
        (int)src.l, (int)src.u, (int)src.size().x, (int)src.size().y
    };
    const SDL_FRect d = {
        (float)dest.l, (float)dest.u, (float)dest.size().x, (float)dest.size().y
    };

    int f = SDL_FLIP_NONE;
    if (flip.x < 0) {
        f |= SDL_FLIP_HORIZONTAL;
    }
    if (flip.y < 0) {
        f |= SDL_FLIP_VERTICAL;
    }

    SDL_RenderCopyExF(rend, t->texture, &s, &d, ang, NULL, (SDL_RendererFlip)f);
}

void Renderer::tex(TextureH texture, 
                   const vec2& p, 
                   float ang, 
                   const BBox& src) const
{
    tex(texture, p, ang, src, 
        BBox::from(vec2(0, 0), src.size()) + p, vec2(1, 1)
    );
}

void Renderer::tex(TextureH texture, 
                   const vec2& p, 
                   float ang) const 
{
    const Texture* const t = res_mng->texture(texture);
    tex(texture, p, ang, BBox::from(0, 0, t->h, t->w));
}


void Renderer::tex_sized(TextureH texture,
                         const vec2& p,
                         float ang,
                         const BBox& src,
                         const vec2& scale) const
{
    tex(texture, p, ang, src, 
        BBox::from(vec2(0, 0), scale.abs()) + p, scale
    );
}
void Renderer::tex_sized(TextureH texture,
                         const vec2& p,
                         float ang,
                         const vec2& scale) const
{
    const Texture* const t = res_mng->texture(texture);
    tex_sized(texture, p, ang, BBox::from(0, 0, t->h, t->w), scale);
}

void Renderer::rect(const BBox& bbox, SDL_Color col) const {
    SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
    SDL_FRect r;
    r.x = (float)bbox.l;
    r.y = (float)bbox.u;
    r.w = (float)bbox.size().x;
    r.h = (float)bbox.size().y;
    SDL_RenderDrawRectF(rend, &r);
}
void Renderer::rectf(const BBox& bbox, SDL_Color col) const {
    SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
    SDL_FRect r;
    r.x = (float)bbox.l;
    r.y = (float)bbox.u;
    r.w = (float)bbox.size().x;
    r.h = (float)bbox.size().y;
    SDL_RenderFillRectF(rend, &r);
}

void Renderer::line(const vec2& a, const vec2& b, SDL_Color col) const {
    SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a); 
    SDL_RenderDrawLineF(rend, (float)a.x, (float)a.y, (float)b.x, (float)b.y);
}

void Renderer::text(const vec2& pos, FontH font, const string2& s) const {
    Font* f = res_mng->font(font);
    Texture* t = res_mng->texture(f->tex);

    const int chars_per_row = t->w / f->sz.x;
    const int chars_per_col = t->h / f->sz.y;

    vec2 p = pos;

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];

        if (c == '\n') {
            p.x = pos.x;
            p.y += f->sz.y;
            continue;
        }

        int src_x = c % chars_per_row;
        int src_y = c / chars_per_row;

        tex(f->tex, p, 0, BBox::from(vec2(src_x, src_y), f->sz)); 
        p.x += f->sz.x;
    }
}