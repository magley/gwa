#pragma once

class Input {
private:
    unsigned char* kb_curr;
    unsigned char* kb_prev;
    int m_x_curr, m_y_curr;
    int m_x_prev, m_y_prev;
    unsigned int m_curr, m_prev;

    void update(const unsigned char* kbdat, unsigned int mdat, int mx, int my);

public:
    Input();
    void update();
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
};