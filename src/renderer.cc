#include "renderer.hh"

#include "gfx.hh"
#include "image_util.hh"
#include "shader.hh"
#include <glm/ext.hpp>
#include <glm/glm.hpp>

// #define DEBUG_MODE
// #define DEBUG_CHUNKS
// #define DEBUG_CAMERA

static const GLfloat debug_translucent_color[] = {1.0f, 1.0f, 1.0f, 0.3f};
static const GLfloat debug_white_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
static const GLfloat debug_ray[] = {
  0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 255.0f
};

static GLfloat tri_data[] = {
  0.0f, 0.0f, 0.0f,
  16.0f, 0.0f, 0.0f,
  16.0f, 255.0f, 0.0f,

  0.0f, 0.0f, 0.0f,
  0.0f, 255.0f, 0.0f,
  16.0f, 255.0f, 0.0f,

  0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 16.0f,
  0.0f, 255.0f, 16.0f,

  0.0f, 0.0f, 0.0f,
  0.0f, 255.0f, 0.0f,
  0.0f, 255.0f, 16.0f,

  0.0f, 0.0f, 16.0f,
  16.0f, 0.0f, 16.0f,
  16.0f, 255.0f, 16.0f,

  0.0f, 0.0f, 16.0f,
  0.0f, 255.0f, 16.0f,
  16.0f, 255.0f, 16.0f,

  16.0f, 0.0f, 0.0f,
  16.0f, 0.0f, 16.0f,
  16.0f, 255.0f, 16.0f,

  16.0f, 0.0f, 0.0f,
  16.0f, 255.0f, 0.0f,
  16.0f, 255.0f, 16.0f,

};

Renderer::Renderer(Window* window)
  : window(window),
  debug_camera_vbo(GL_ARRAY_BUFFER, false),
  debug_lines_vbo(GL_ARRAY_BUFFER, false) {
  for (int x = -5; x <= 5; x++) {
    for (int y = -5; y <= 5; y++) {
      chunks.push_back(*new Chunk(x, y));
    }
  }
}

void Renderer::init() {
  chunk_shader.load("res/shaders/chunk.vert", "res/shaders/chunk.frag");
  u_projection = chunk_shader.getUniform("projection");
  u_view = chunk_shader.getUniform("view");
  u_model = chunk_shader.getUniform("model");
  u_texture = chunk_shader.getUniform("u_texture");
  u_camera = simple_shader.getUniform("u_camera");

  simple_shader.load("res/shaders/simple.vert", "res/shaders/simple.frag");
  s_projection = simple_shader.getUniform("projection");
  s_view = simple_shader.getUniform("view");
  s_model = simple_shader.getUniform("model");
  s_color = simple_shader.getUniform("u_color");

  debug_lines_vao.init();
  debug_lines_vao.bind();

  debug_lines_vbo.init();
  debug_camera_vbo.init();

  debug_lines_vbo.write((GLfloat*) tri_data, 0, sizeof(tri_data));
  debug_camera_vbo.write((GLfloat*) debug_ray, 0, sizeof(debug_ray));

  for (auto chunk : chunks) {
    chunk.get().mesh.value().get().init();
    chunk.get().mesh.value().get().gen(*window);
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  block_tex = load_bmp("res/images/tex3.bmp");
  // glEnable(GL_CULL_FACE);

}


void Renderer::render() {

  chunk_shader.use();
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &window->camera->projection_matrix[0][0]);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, &window->camera->view_matrix[0][0]);
  glUniform3fv(u_camera, 1, &window->camera->position[0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, block_tex);
  // Set our "myTextureSampler" sampler to use Texture Unit 0
  glUniform1i(u_texture, 0);

  for (auto chunk : chunks) {
    chunk.get().mesh.value().get().render(*window);
  }

#ifdef DEBUG_MODE
  simple_shader.use();
  glUniformMatrix4fv(s_projection, 1, GL_FALSE, &window->camera->projection_matrix[0][0]);
  glUniformMatrix4fv(s_view, 1, GL_FALSE, &window->camera->view_matrix[0][0]);
#endif

#ifdef DEBUG_CHUNKS
  glUniform4fv(s_color, 1, debug_translucent_color);

  auto model_matrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(
          floor(window->camera->position.x / CHUNK_SIZE) * CHUNK_SIZE,
          0.0f,
          floor(window->camera->position.z / CHUNK_SIZE) * CHUNK_SIZE
        )
      );

  glUniformMatrix4fv(s_model, 1, GL_FALSE, &model_matrix[0][0]);

  debug_lines_vao.attr(debug_lines_vbo, 0, 3, GL_FLOAT, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 24);
#endif

#ifdef DEBUG_CAMERA
  glUniform4fv(s_color, 1, debug_white_color);

  auto cam_model_matrix =
    glm::translate(glm::mat4(1.0f), window->camera->position);
    //glm::rotate(glm::mat4(1.0f), window->camera->rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));

  glUniformMatrix4fv(s_model, 1, GL_FALSE, &cam_model_matrix[0][0]);

  debug_lines_vao.attr(debug_camera_vbo, 0, 3, GL_FLOAT, 0, 0);

  glDrawArrays(GL_LINES, 0, 2);

#endif
}

inline int modulo(int x,int N){
    return (x % N + N) %N;
}

std::reference_wrapper<Chunk> Renderer::getChunk(int x, int z) {
  for (auto chunk : chunks) {
    if (chunk.get().x == x && chunk.get().z == z) {
      return chunk;
    }
  }
  throw std::runtime_error("Chunk access out of bounds");
}

Block& Renderer::getAbsoluteBlock(int x, int y, int z) {
  int cx = (int) floor((float) x / (float) CHUNK_SIZE);
  int cz = (int) floor((float) z / (float) CHUNK_SIZE);
  int bx = (int) modulo(x, CHUNK_SIZE);
  int bz = (int) modulo(z, CHUNK_SIZE);

  // if (x < 0) {
  //   bx = (CHUNK_SIZE-1) + bx;
  // }
  // if (z < 0) {
  //   bz = (CHUNK_SIZE-1) + bz;
  // }

  for (auto chunk : chunks) {
    if (chunk.get().x == cx && chunk.get().z == cz) {
      try {
        return chunk.get().getBlock(bx, y, bz);
      } catch (std::exception& e) {
        printf("U suck at coding (renderer.cc:79): %d, %d, %d\n", bx, y, bz);
      }
    }
  }
  return *new Block {0x1};
}

