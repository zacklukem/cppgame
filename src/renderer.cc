#include "renderer.hh"

#include "gfx.hh"
#include "image_util.hh"
#include "shader.hh"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include "world_render.hh"
#include "debug_render.hh"
#include "hud_render.hh"

Renderer::Renderer(Window* window)
  : window(window) {
  passes.push_back(std::make_unique<WorldRenderer>());
  passes.push_back(std::make_unique<DebugRenderer>());
  passes.push_back(std::make_unique<HUDRenderer>());
}

void Renderer::init() {

  for (auto& pass : passes) {
    pass.get()->init(*window);
  }
  // glEnable(GL_CULL_FACE);

}


void Renderer::render() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (auto& pass : passes) {
    pass.get()->render(*window);
  }
}
