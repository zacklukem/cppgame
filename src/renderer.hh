#pragma once

class Renderer;

#include "window.hh"
#include "shader.hh"
#include "vao.hh"
#include "vbo.hh"
#include "chunk.hh"

class Renderer {
  public:
    Renderer(Window* window);
    void init();
    void render();
    Block& getAbsoluteBlock(int x, int y, int z);
    Shader chunk_shader;
    GLint u_model, u_view, u_projection, u_texture;
    GLuint block_tex;
    std::vector<std::reference_wrapper<Chunk>> chunks;
  private:
    Window* window;
};

