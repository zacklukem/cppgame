#pragma once

#include "renderer.hh"

class WorldRenderer : public RenderPass {
  public:
    WorldRenderer() = default;
    void init(Window& window) override;
    void render(Window& window) override;
    ~WorldRenderer() override = default;
  private:
    Shader chunk_shader;
    GLint u_model, u_view, u_projection, u_camera, u_texture;
    GLuint block_tex;
};
