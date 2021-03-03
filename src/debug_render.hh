#pragma once

#include "renderer.hh"

class DebugRenderer : public RenderPass {
  public:
    DebugRenderer();
    void init(Window& window) override;
    void render(Window& window) override;
    ~DebugRenderer() override = default;
  private:
    Shader simple_shader;
    GLint s_model, s_view, s_projection, s_color;
    VAO debug_lines_vao;
    VBO debug_lines_vbo;
    VBO debug_camera_vbo;
};
