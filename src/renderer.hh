#pragma once
#include <memory>

class Renderer;

#include "window.hh"
#include "shader.hh"
#include "vao.hh"
#include "vbo.hh"
#include "chunk.hh"

class RenderPass {
  public:
    virtual void init(Window& window) = 0;
    virtual void render(Window& window) = 0;
    virtual ~RenderPass() {};
};

class Renderer {
  public:
    Renderer(Window* window);
    void init();
    void render();
  private:
    Window* window;
    std::vector<std::unique_ptr<RenderPass>> passes;
};

