#include "world_render.hh"
#include "image_util.hh"

void WorldRenderer::init(Window& window) {
  chunk_shader.load("res/shaders/chunk.vert", "res/shaders/chunk.frag");
  u_projection = chunk_shader.getUniform("projection");
  u_view = chunk_shader.getUniform("view");
  u_model = chunk_shader.getUniform("model");
  u_texture = chunk_shader.getUniform("u_texture");
  u_camera = chunk_shader.getUniform("u_camera");

  block_tex = load_image("res/images/blocks.png");

  for (auto chunk : window.world.chunks) {
    chunk.get().mesh.value().get().init();
    chunk.get().mesh.value().get().gen(window);
  }

}

void WorldRenderer::render(Window& window) {
  // glPolygonMode(GL_FRONT_AND_BACK, self->flags.wireframe ? GL_LINE : GL_FILL);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  chunk_shader.use();
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &window.camera->projection_matrix[0][0]);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, &window.camera->view_matrix[0][0]);
  glUniform3fv(u_camera, 1, &window.camera->position[0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, block_tex);
  // Set our "myTextureSampler" sampler to use Texture Unit 0
  glUniform1i(u_texture, 0);

  for (auto chunk : window.world.chunks) {
    chunk.get().mesh.value().get().render(window, u_model);
  }

}
