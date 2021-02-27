#include "renderer.hh"

#include "gfx.hh"
#include "image_util.hh"
#include "shader.hh"

static const GLfloat tri_data[] = {
  -1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  0.0f, 1.0f, 0.0f,

  -1.0f, -1.0f, 1.0f,
  1.0f, -1.0f, 1.0f,
  0.0f, 1.0f, 0.0f
};

Renderer::Renderer(Window* window)
  : window(window) {
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      chunks.push_back(*new Chunk(x, y));
    }
  }
}

void Renderer::init() {
  chunk_shader.load("res/shaders/shader.vert", "res/shaders/shader.frag");
  chunk_shader.use();
  u_projection = chunk_shader.getUniform("projection");
  u_view = chunk_shader.getUniform("view");
  u_model = chunk_shader.getUniform("model");
  u_texture = chunk_shader.getUniform("u_texture");

  for (auto chunk : chunks) {
    chunk.get().mesh.value().get().init();
    chunk.get().mesh.value().get().gen(*window);
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  block_tex = load_bmp("res/images/tex3.bmp");
  // glEnable(GL_CULL_FACE);

}

void Renderer::render() {

  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &window->camera->projection_matrix[0][0]);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, &window->camera->view_matrix[0][0]);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, block_tex);
  // Set our "myTextureSampler" sampler to use Texture Unit 0
  glUniform1i(u_texture, 0);

  for (auto chunk : chunks) {
    chunk.get().mesh.value().get().render(*window);
  }
}

Block& Renderer::getAbsoluteBlock(int x, int y, int z) {
  int cx = (int) (x / CHUNK_SIZE);
  int cz = (int) (z / CHUNK_SIZE);
  int bx = (int) (x % CHUNK_SIZE);
  int bz = (int) (z % CHUNK_SIZE);

  if (x < 0) {
    bx = (CHUNK_SIZE-1) + bx;
  }
  if (z < 0) {
    bz = (CHUNK_SIZE-1) + bz;
  }

  for (auto chunk : chunks) {
    if (chunk.get().x == cx && chunk.get().z == cz) {
      try {
        return chunk.get().getBlock(bx, y, bz);
      } catch (std::exception& e) {
        printf("U suck at coding: %d, %d\n", bx, bz);
      }
    }
  }
  return *new Block {0x0};
}

