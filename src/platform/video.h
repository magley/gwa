#pragma once

struct SDL_Texture;
struct SDL_Renderer;

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
public:
    Renderer(SDL_Renderer* rend);
    int clear(unsigned char r, unsigned char g, unsigned char b) const;
    void flip() const;
    void draw(const Texture& tex, float x, float y, float ang = 0) const;
    void draw_ext(const Texture& tex, float x, float y, float ang = 0,
                  bool flipx = false, bool flipy = false, int sx = 0, 
                  int sy = 0, int sw = -1, int sh = -1) const;
};