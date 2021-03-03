#pragma once

class ChunkMesh;

#include <glm/glm.hpp>
#include "chunk.hh"
#include "vbo.hh"
#include "vao.hh"
#include "window.hh"
#include "block.hh"

class ChunkMesh {
  public:
    ChunkMesh(Chunk& chunk);
    void init();
    void gen(Window& window);
    void render(Window& window, GLint u_model);
    Chunk& chunk;
  private:
    Block& getWorldBlock(Window& window, int x, int y, int z);
    VAO vao;
    VBO vertices;
    VBO indices;
    GLsizei num_elements;
    glm::mat4 model_matrix;
};
