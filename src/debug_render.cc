#include "debug_render.hh"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

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

DebugRenderer::DebugRenderer() :
  debug_camera_vbo(GL_ARRAY_BUFFER, false),
  debug_lines_vbo(GL_ARRAY_BUFFER, false) {}

void DebugRenderer::init(Window& window) {
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

}

void DebugRenderer::render(Window& window) {
#ifdef DEBUG_MODE
  simple_shader.use();
  glUniformMatrix4fv(s_projection, 1, GL_FALSE, &window.camera->projection_matrix[0][0]);
  glUniformMatrix4fv(s_view, 1, GL_FALSE, &window.camera->view_matrix[0][0]);
#endif

#ifdef DEBUG_CHUNKS
  glUniform4fv(s_color, 1, debug_translucent_color);

  auto model_matrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(
          floor(window.camera->position.x / CHUNK_SIZE) * CHUNK_SIZE,
          0.0f,
          floor(window.camera->position.z / CHUNK_SIZE) * CHUNK_SIZE
        )
      );

  glUniformMatrix4fv(s_model, 1, GL_FALSE, &model_matrix[0][0]);

  debug_lines_vao.attr(debug_lines_vbo, 0, 3, GL_FLOAT, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 24);
#endif

#ifdef DEBUG_CAMERA
  glUniform4fv(s_color, 1, debug_white_color);

  auto cam_model_matrix =
    glm::translate(glm::mat4(1.0f), window.camera->position);
    //glm::rotate(glm::mat4(1.0f), window->camera->rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));

  glUniformMatrix4fv(s_model, 1, GL_FALSE, &cam_model_matrix[0][0]);

  debug_lines_vao.attr(debug_camera_vbo, 0, 3, GL_FLOAT, 0, 0);

  glDrawArrays(GL_LINES, 0, 2);

#endif

}
