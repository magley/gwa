#include "input.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include "ctx/ctx.h"

Input::Input() {
    kb_curr = new unsigned char[255];
    kb_prev = new unsigned char[255];
}

void Input::init_ctx(GwaCtx* ctx) {
    this->ctx = ctx;
}

void Input::update(const unsigned char* kb_data,
                   unsigned int m_data,
                   int mx,
                   int my) {
    for (int i = 0; i < 255; i++) {
        kb_prev[i] = kb_curr[i];
        kb_curr[i] = kb_data[i];
    }

    m_prev = m_curr;
    m_curr = m_data;

    m_x_prev = m_x_curr;
    m_y_prev = m_y_curr;
    m_x_curr = mx;
    m_y_curr = my;
}

void Input::update_wheel(int wheel_dy) {
    m_wheel = wheel_dy;
}

void Input::update() {
    const unsigned char* kb_state = SDL_GetKeyboardState(NULL);
    int mx, my;
    const unsigned int m_state = SDL_GetMouseState(&mx, &my);

    update(kb_state, m_state, mx, my);
}

bool Input::down(unsigned char scancode) const {
    return kb_curr[scancode];
}

bool Input::press(unsigned char scancode) const {
    return kb_curr[scancode] && !kb_prev[scancode];
}

bool Input::release(unsigned char scancode) const {
    return !kb_curr[scancode] && kb_prev[scancode];
}

bool Input::m_down(unsigned int btn_mask) const {
    return (m_curr & btn_mask) == btn_mask;
}

bool Input::m_press(unsigned int btn_mask) const {
    return (m_curr & btn_mask) == btn_mask && (m_prev & btn_mask) != btn_mask;
}

bool Input::m_release(unsigned int btn_mask) const {
    return (m_curr & btn_mask) != btn_mask && (m_prev & btn_mask) == btn_mask;
}

int Input::m_x() const {
    return m_x_curr;
}

int Input::m_y() const {
    return m_y_curr;
}

int Input::m_vx() const {
    return m_x_curr - m_x_prev;
}

int Input::m_vy() const {
    return m_y_curr - m_y_prev;
}

int Input::m_w() const {
    return m_wheel;
}

vec2 Input::m_pos() const {
    return vec2(m_x_curr, m_y_curr) / ctx->scale;
}

vec2 Input::m_pos_raw() const {
    return vec2(m_x_curr, m_y_curr);
}