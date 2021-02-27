#pragma once

struct Mouse;
struct Keyboard;

#include "gfx.hh"
#include "window.hh"

struct Mouse {
  glm::vec2 delta;
  glm::vec2 position;
};

struct Button {
    bool down, last, last_tick, pressed, pressed_tick;
};

struct Keyboard {
    struct Button keys[GLFW_KEY_LAST];
};

void init_inputs(Window& window);
void set_cursor_grabbed(Window& window, bool grabbed);
void update_cursor(Window& window);
