#include "hud_render.hh"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "image_util.hh"
#include "block.hh"

#define DOTW 15.0f
#define BLOCK_SEL_WIDTH 50.0f

struct Vertex {
  float x, y;
  float ux, uy;
};

HUDRenderer::HUDRenderer()
  : vbo(GL_ARRAY_BUFFER, true),
  ibo(GL_ARRAY_BUFFER, true) {

}

void HUDRenderer::init(Window& window) {

  view = glm::mat4(1.0f);
  projection = glm::ortho(0.0f, (float) window.width, 0.0f, (float) window.height, -100.0f, 100.0f);

  hud_shader.load("res/shaders/ui.vert", "res/shaders/ui.frag");
  u_projection = hud_shader.getUniform("projection");
  u_view = hud_shader.getUniform("view");
  u_model = hud_shader.getUniform("model");
  u_texture = hud_shader.getUniform("u_texture");

  crosshair_tex = load_image("res/images/crosshair.png");

  vao.init();
  vao.bind();

  vbo.init();
  ibo.init();

}

void HUDRenderer::renderQuad(glm::vec2 pos, glm::vec2 size, glm::vec2 u_start, glm::vec2 u_end) {
  glm::mat4 model = glm::translate(
    glm::mat4(1.0f),
    glm::vec3(pos, 0.0f)
  );
  glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);

  Vertex vbo_data[] = {
    {0, 0, u_start.x, u_start.y},
    {0, size.y, u_start.x, u_end.y},
    {size.x, size.y, u_end.x, u_end.y},
    {size.x, 0, u_end.x, u_start.y}
  };

  GLuint ibo_data[] = {
    0, 1, 2,
    0, 2, 3
  };

  vbo.write((GLfloat*) vbo_data, 0, sizeof(vbo_data));
  ibo.write((GLuint*) ibo_data, 0, sizeof(ibo_data));

  vao.attr(vbo, 0, 2, GL_FLOAT, sizeof(Vertex), 0);
  vao.attr(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), sizeof(float) * 2);

  ibo.bind();

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);
}

void HUDRenderer::render(Window& window) {
  // glClear(GL_DEPTH_BUFFER_BIT);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  hud_shader.use();
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);

  // this jank. assumes that tex0 is still loaded
  glUniform1i(u_texture, 0);
  for (int i = 1; i < 9; i++) {
    glm::vec2* uv = getUVCoords(* new Block {(unsigned short) i}, NORTH);
    renderQuad(
        glm::vec2((i-1) * (BLOCK_SEL_WIDTH), 0),
        glm::vec2(BLOCK_SEL_WIDTH, BLOCK_SEL_WIDTH),
        glm::vec2(uv[0].x, uv[0].y),
        glm::vec2(uv[2].x, uv[2].y)
      );

  }

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, crosshair_tex);
  glUniform1i(u_texture, 1);
  renderQuad(
      glm::vec2(window.width/2.0f - DOTW/2.0f, window.height/2.0f - DOTW/2.0f),
      glm::vec2(DOTW, DOTW),
      glm::vec2(0.0f, 0.0f),
      glm::vec2(1.0f, 1.0f)
    );

}

