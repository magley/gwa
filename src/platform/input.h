#pragma once

struct vec2;
#include "ctx/fdecl.h"

class Input {
private:
    GwaCtx* ctx; // Required to get scaled mousepos (window scale).
    unsigned char* kb_curr;
    unsigned char* kb_prev;
    int m_x_curr, m_y_curr;
    int m_x_prev, m_y_prev;
    int m_wheel;
    unsigned int m_curr, m_prev;

    void update(const unsigned char* kbdat, unsigned int mdat, int mx, int my);

public:
    Input();
    void init_ctx(GwaCtx* ctx);
    void update();
    void update_wheel(int wheel_dy);
    bool down(unsigned char scancode) const;
    bool press(unsigned char scancode) const;
    bool release(unsigned char scancode) const;
    bool m_down(unsigned int btn_mask) const;
    bool m_press(unsigned int btn_mask) const;
    bool m_release(unsigned int btn_mask) const;
    int m_x() const;
    int m_y() const;
    int m_vx() const;
    int m_vy() const;
    int m_w() const;
    vec2 m_pos() const;
    vec2 m_pos_raw() const;
};