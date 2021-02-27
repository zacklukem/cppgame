#pragma once

#include "gfx.hh"
#include <optional>
#include <functional>

class Window;

#include "camera.hh"
#include "renderer.hh"
#include "input.hh"

class Window {
  public:
    Window(int width, int height, const char* title);
    ~Window();
    void start();
    void setRenderer(Renderer* renderer);

    GLFWwindow* glfw_window;
    int width, height;
    float dt;
    Camera* camera;
    Mouse& mouse;
    Keyboard& keyboard;
    // Maybe be more careful with pointers...
    Renderer* renderer = NULL;
};

