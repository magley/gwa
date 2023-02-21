#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <assert.h>

void print_err(const char* prefix, const char* msg, const char* suffix) {
    printf("\033[0;37m%s\033[0;31m%s\033[0;37m%s", prefix, msg, suffix);
}

void print_sdl_error() {
    const char* error = SDL_GetError();
    print_err("SDL2: ", error, "\n");
}

void handle_sdl_error() {
    print_sdl_error();
    exit(1);
}

class SDLInputWrapper {
    const unsigned char* kb_curr = kb_prev; // Note the initialization.
    unsigned char kb_prev[255];
    int m_x_curr, m_y_curr;
    int m_x_prev, m_y_prev;
    unsigned int m_curr, m_prev;

    void update(const unsigned char* kb_data, 
                unsigned int m_data, 
                int mx, 
                int my) 
    {
        for (int i = 0; i < 255; i++) {
            kb_prev[i] = kb_curr[i];
        }
        kb_curr = kb_data;

        m_prev = m_curr;
        m_curr = m_data;

        m_x_prev = m_x_curr;
        m_y_prev = m_y_curr;
        m_x_curr = mx;
        m_y_curr = my;
    }
public:
    void update() {
        const unsigned char* kb_state = SDL_GetKeyboardState(NULL);
        int mx, my;
        const unsigned int m_state = SDL_GetMouseState(&mx, &my);

        update(kb_state, m_state, mx, my);
    }

    bool down(unsigned char scancode) const {
        return kb_curr[scancode];
    }

    bool press(unsigned char scancode) const {
        return kb_curr[scancode] && !kb_prev[scancode];
    }

    bool release(unsigned char scancode) const {
        return !kb_curr[scancode] && kb_prev[scancode];
    }

    bool m_down(unsigned int btn_mask) const {
        return (m_curr & btn_mask) == btn_mask;
    }

    bool m_press(unsigned int btn_mask) const {
        return (m_curr & btn_mask) == btn_mask 
            && (m_prev & btn_mask) != btn_mask;
    }

    bool m_release(unsigned int btn_mask) const {
        return (m_curr & btn_mask) != btn_mask 
            && (m_prev & btn_mask) == btn_mask;
    }

    int m_x() const { 
        return m_x_curr; 
    }

    int m_y() const { 
        return m_y_curr; 
    }

    int m_vx() const {
        return m_x_curr - m_x_prev;
    }

    int m_vy() const {
        return m_y_curr - m_y_prev;
    }
};

class Texture {
public:
    SDL_Texture* texture = NULL;
    int w, h;

    Texture() {}
    Texture(SDL_Renderer* rend, const char* fname) {
        texture = IMG_LoadTexture(rend, fname);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    }

    void draw(SDL_Renderer* rend, int x, int y) {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        dest.w = w;
        dest.h = h;
        SDL_RenderCopy(rend, texture, NULL, &dest);
    }
};

int main(int argc, char** argv) {
    int status = SDL_Init(SDL_INIT_VIDEO);
    if (status != 0) {
        handle_sdl_error();
    }

    status = IMG_Init(IMG_INIT_PNG);
    if (status == 0) {
        handle_sdl_error();
    }


    SDLInputWrapper input;

    SDL_Window* win = SDL_CreateWindow("gwa", 
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED, 
                                        640, 480, SDL_WINDOW_OPENGL);
    if (win == NULL) {
        handle_sdl_error();
    }

    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
                                                    SDL_RENDERER_PRESENTVSYNC);
    if (rend == NULL) {
        handle_sdl_error();
    }

    Texture tex = Texture(rend, "../res/img.png");

    int x = 0;
    int y = 0;

    SDL_Event event;
    bool is_running = true;

    while (is_running) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

        input.update();

        x += (input.down(SDL_SCANCODE_RIGHT)-input.down(SDL_SCANCODE_LEFT)) * 3;
        y += (input.down(SDL_SCANCODE_DOWN)-input.down(SDL_SCANCODE_UP)) * 3;

        status = SDL_SetRenderDrawColor(rend, 128, 128, 128, 255);
        if (status != 0) {
            handle_sdl_error();
        }

        status = SDL_RenderClear(rend);
        if (status != 0) {
            handle_sdl_error();
        }

        tex.draw(rend, x, y);

        SDL_RenderPresent(rend);
    }

    SDL_Quit();
    return 0;
}