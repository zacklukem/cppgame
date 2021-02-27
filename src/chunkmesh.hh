#pragma once

class ChunkMesh;

#include <glm/glm.hpp>
#include "chunk.hh"
#include "vbo.hh"
#include "vao.hh"
#include "window.hh"

// +y 010
#define UP 0
// -y -010
#define DOWN 1
// +z 001
#define NORTH 2
// -z -001
#define SOUTH 3
// -x -100
#define EAST 4
// +x 100
#define WEST 5

struct Vertex {
  float x, y, z;
  float ux, uy;
  std::uint32_t data;
};

struct TriIndex {
  GLuint a, b, c;
};

class ChunkMesh {
  public:
    ChunkMesh(Chunk& chunk);
    void init();
    void gen(Window& window);
    void render(Window& window);
    Chunk& chunk;
  private:
    Block& getWorldBlock(Window& window, int x, int y, int z);
    VAO vao;
    VBO vertices;
    VBO indices;
    GLsizei num_elements;
    glm::mat4 model_matrix;
};
