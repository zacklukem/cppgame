#include "chunkmesh.hh"

#include "window.hh"
#include <glm/ext.hpp>

struct Vertex {
  float x, y, z;
  float ux, uy;
  std::uint32_t data;
};

struct TriIndex {
  GLuint a, b, c;
};


ChunkMesh::ChunkMesh(Chunk& chunk)
  : chunk(chunk),
  vertices(GL_ARRAY_BUFFER, false),
  indices(GL_ELEMENT_ARRAY_BUFFER, false) {}

void ChunkMesh::init() {
  vao.init();
  vao.bind();

  vertices.init();
  indices.init();

  model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(chunk.x * CHUNK_SIZE, 0, chunk.z * CHUNK_SIZE));

}

Block& ChunkMesh::getWorldBlock(Window& window, int x, int y, int z) {
  if (x > 0 &&
      x < (CHUNK_SIZE - 1) &&
      y > 0 &&
      y < (CHUNK_HEIGHT - 1) &&
      z > 0 &&
      z < (CHUNK_SIZE - 1)) {
    return chunk.getBlock(x, y, z);
  }

  int ax = chunk.x * 16 + x;
  int az = chunk.z * 16 + z;

  return window.world.getAbsoluteBlock(ax, y, az);

}

void ChunkMesh::gen(Window& window) {
  std::vector<Vertex> vert_vec;
  std::vector<TriIndex> i_vec;

  for (int x = 0; x < CHUNK_SIZE; x++) {
    for (int y = 0; y < CHUNK_HEIGHT; y++) {
      for (int z = 0; z < CHUNK_SIZE; z++) {
        Block& block = chunk.getBlock(x, y, z);
        if (block.id) {
          float a = (float) rand() / RAND_MAX;
          float b = (float) rand() / RAND_MAX;
          for (GLuint face = 0; face < 6; face++) {
            GLuint vert_i = vert_vec.size();
            try {
              if (face == EAST && getWorldBlock(window, x-1, y, z).id)
                continue;
              if (face == WEST && getWorldBlock(window, x+1, y, z).id)
                continue;
              if (face == SOUTH && getWorldBlock(window, x, y, z-1).id)
                continue;
              if (face == NORTH && getWorldBlock(window, x, y, z+1).id)
                continue;
              if (face == DOWN && y > 0 && chunk.getBlock(x, y-1, z).id)
                continue;
              if (face == UP && y < (CHUNK_HEIGHT-1) && chunk.getBlock(x, y+1, z).id)
                continue;
              if (face == DOWN && y == 0)
                continue;
            } catch (std::out_of_range& e) {
              printf("Out of range face: %d, %s\n", face, e.what());
            }
            glm::vec2* uv = getUVCoords(block, face);
            switch (face) {
              case UP:
              case DOWN:
                vert_vec.push_back({x+0.0f, y+(face==UP?1.0f:0.0f), z+0.0f, uv[0].x, uv[0].y, face});
                vert_vec.push_back({x+1.0f, y+(face==UP?1.0f:0.0f), z+0.0f, uv[1].x, uv[1].y, face});
                vert_vec.push_back({x+1.0f, y+(face==UP?1.0f:0.0f), z+1.0f, uv[2].x, uv[2].y, face});
                vert_vec.push_back({x+0.0f, y+(face==UP?1.0f:0.0f), z+1.0f, uv[3].x, uv[3].y, face});
                break;
              case EAST:
              case WEST:
                vert_vec.push_back({x+(face==WEST?1.0f:0.0f), y+0.0f, z+0.0f, uv[0].x, uv[0].y, face});
                vert_vec.push_back({x+(face==WEST?1.0f:0.0f), y+1.0f, z+0.0f, uv[1].x, uv[1].y, face});
                vert_vec.push_back({x+(face==WEST?1.0f:0.0f), y+1.0f, z+1.0f, uv[2].x, uv[2].y, face});
                vert_vec.push_back({x+(face==WEST?1.0f:0.0f), y+0.0f, z+1.0f, uv[3].x, uv[3].y, face});
                break;
              case NORTH:
              case SOUTH:
                vert_vec.push_back({x+0.0f, y+0.0f, z+(face==NORTH?1.0f:0.0f), uv[0].x, uv[0].y, face});
                vert_vec.push_back({x+1.0f, y+0.0f, z+(face==NORTH?1.0f:0.0f), uv[1].x, uv[1].y, face});
                vert_vec.push_back({x+1.0f, y+1.0f, z+(face==NORTH?1.0f:0.0f), uv[2].x, uv[2].y, face});
                vert_vec.push_back({x+0.0f, y+1.0f, z+(face==NORTH?1.0f:0.0f), uv[3].x, uv[3].y, face});
                break;
            }
            i_vec.push_back({vert_i + 0, vert_i + 1, vert_i + 2});
            i_vec.push_back({vert_i + 0, vert_i + 2, vert_i + 3});
          }
        }
      }
    }
  }

  num_elements = i_vec.size() * 3;

  // printf("Vertices: %d\n", num_elements);

  vertices.write((GLfloat*) vert_vec.data(), 0, sizeof(Vertex) * vert_vec.size());
  indices.write((GLuint*) i_vec.data(), 0, sizeof(TriIndex) * i_vec.size());


}

void ChunkMesh::render(Window& window, GLint u_model) {

  // set model pos uniform
  glUniformMatrix4fv(u_model, 1, GL_FALSE, &model_matrix[0][0]);

  vao.attr(vertices, 0, 3, GL_FLOAT, sizeof(Vertex), 0);
  vao.attr(vertices, 1, 2, GL_FLOAT, sizeof(Vertex), sizeof(float) * 3);
  vao.attr(vertices, 2, 1, GL_UNSIGNED_INT, sizeof(Vertex), sizeof(float) * 5);

  indices.bind();

  glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, (void*) 0);

  glDisableVertexAttribArray(0);

}

