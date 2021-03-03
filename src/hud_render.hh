#pragma once

#include "renderer.hh"

class HUDRenderer : public RenderPass {
  public:
    HUDRenderer();
    void init(Window& window) override;
    void render(Window& window) override;
    ~HUDRenderer() override = default;
  private:
    void renderQuad(glm::vec2 pos, glm::vec2 size, glm::vec2 u_start, glm::vec2 u_end);
    Shader hud_shader;
    GLint u_projection, u_view, u_model, u_texture;
    VAO vao;
    VBO vbo, ibo;
    glm::mat4 projection;
    glm::mat4 view;
    GLuint crosshair_tex;
};
