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
    std::reference_wrapper<Chunk> getChunk(int x, int y);
    Shader chunk_shader;
    Shader simple_shader;
    GLint u_model, u_view, u_projection, u_camera, u_texture;
    GLint s_model, s_view, s_projection, s_color;
    GLuint block_tex;
    std::vector<std::reference_wrapper<Chunk>> chunks;
    VAO debug_lines_vao;
    VBO debug_lines_vbo;
    VBO debug_camera_vbo;
  private:
    Window* window;
};

