#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <functional>

class Camera;

#include "window.hh"


class Camera {
public:
  Camera();
  void calculateProjectionMatrix();
  void update();
  void setWindow(Window& window);
  void castRay(int button);
  glm::mat4 projection_matrix;
  glm::mat4 view_matrix;
  glm::vec3 position;
  glm::vec2 rotation;
private:
  std::optional<std::reference_wrapper<Window>> window = std::nullopt;
};

