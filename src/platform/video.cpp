#include "video.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#include "util/geometry/bbox.h"
#include "util/geometry/vec2.h"

#include "resource/res_mng.h"
#include "util/string/string2.h"
#include "font.h"
#include "ctx/ctx.h"

void Renderer::init_ctx(GwaCtx* ctx) {
    this->ctx = ctx;
}

void Renderer::begin_gui() {
    scale = ctx->scale;
}

void Renderer::end_gui() {
    scale = 1;
}

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

void Renderer::swap_buffers() const {
    SDL_RenderPresent(rend);
    SDL_Delay(10);
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
    Texture* t = ctx->rm->texture(texture);

    const SDL_Rect s = {
        (int)src.l, (int)src.u, (int)src.size().x, (int)src.size().y
    };
    const SDL_FRect d = {
        (float)dest.l/ scale, (float)dest.u/ scale, (float)dest.size().x/ scale, (float)dest.size().y/ scale
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
    const Texture* const t = ctx->rm->texture(texture);
    tex(texture, p, ang, BBox::from(0, 0, t->h, t->w));
}

void Renderer::tex_colored(TextureH texture, 
                           const vec2& p, 
                           float ang,
                           const BBox& src, 
                           const BBox& dest,
                           const vec2& flip,
                           SDL_Color col) const
{
    Texture* t = ctx->rm->texture(texture);

    SDL_SetTextureColorMod(t->texture, col.r, col.g, col.b);
    SDL_SetTextureAlphaMod(t->texture, col.a);

    const SDL_Rect s = {
        (int)src.l, (int)src.u, (int)src.size().x, (int)src.size().y
    };
    const SDL_FRect d = {
        (float)dest.l/ scale, (float)dest.u/ scale, (float)dest.size().x/ scale, (float)dest.size().y/ scale
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

void Renderer::tex_colored(TextureH texture,
        const vec2& p,
        float ang,
        const BBox& src,
        SDL_Color col) const
{
    tex_colored(texture, p, ang, src, 
        BBox::from(vec2(0, 0), src.size()) + p, vec2(1, 1), col
    );
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
    const Texture* const t = ctx->rm->texture(texture);
    tex_sized(texture, p, ang, BBox::from(0, 0, t->h, t->w), scale);
}

void Renderer::rect(const BBox& bbox, SDL_Color col) const {
    SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
    SDL_FRect r;
    r.x = (float)bbox.l/ scale;
    r.y = (float)bbox.u/ scale;
    r.w = (float)bbox.size().x/ scale;
    r.h = (float)bbox.size().y/ scale;
    SDL_RenderDrawRectF(rend, &r);
}
void Renderer::rectf(const BBox& bbox, SDL_Color col) const {
    SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
    SDL_FRect r;
    r.x = (float)bbox.l/ scale;
    r.y = (float)bbox.u/ scale;
    r.w = (float)bbox.size().x/ scale;
    r.h = (float)bbox.size().y/ scale;
    SDL_RenderFillRectF(rend, &r);
}

void Renderer::line(const vec2& a, const vec2& b, SDL_Color col) const {
    SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a); 
    SDL_RenderDrawLineF(rend, (float)a.x/ scale, (float)a.y/ scale, (float)b.x/ scale, (float)b.y/ scale);
}

void Renderer::text(const vec2& pos, FontH font, const string2& s, SDL_Color col) const {
    Font* f = ctx->rm->font(font);
    Texture* t = ctx->rm->texture(f->tex);

    const int chars_per_row = t->w / f->cell_size.x;
    const int chars_per_col = t->h / f->cell_size.y;

    vec2 p = pos;

    SDL_SetTextureColorMod(t->texture, col.r, col.g, col.b);
    SDL_SetTextureAlphaMod(t->texture, col.a);

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];

        if (c == '\n') {
            p.x = pos.x;
            p.y += f->cell_size.y;
            continue;
        }

        int src_x = c % chars_per_row;
        int src_y = c / chars_per_row;

        tex(f->tex, p, 0, BBox::from(vec2(src_x * f->cell_size.x, src_y * f->cell_size.y), f->cell_size)); 
        p.x += f->base_width[c];
    }
}

vec2 Renderer::text_size(FontH font, const string2& s) const {
    Font* f = ctx->rm->font(font);
    Texture* t = ctx->rm->texture(f->tex);

    const int chars_per_row = t->w / f->cell_size.x;
    const int chars_per_col = t->h / f->cell_size.y;

    vec2 p = vec2(0, 0);
    vec2 pmax = vec2(0, 0);

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];

        if (c == '\n') {
            p.x = 0;
            p.y += f->cell_size.y;
            continue;
        }
        p.x += f->base_width[c];

        pmax.x = pmax.x < p.x ? p.x : pmax.x;
    }

    p.y += f->cell_size.y;

    pmax.y = p.y;

    return pmax;
}