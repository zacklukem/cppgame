#include "camera.hh"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <functional>
#include <stdio.h>

#define SENS 0.1f
#define SPEED 5.0f

Camera::Camera() {
  position = glm::vec3(0.0, 15.0, 0.0f);
  rotation = glm::vec2(0.0, -0.5);
}

void Camera::setWindow(Window& window) {
  this->window = std::optional<std::reference_wrapper<Window>> {window};
}

void Camera::calculateProjectionMatrix() {
  if (!window.has_value()) {
    throw std::runtime_error("Window must be set before calculating the projection matrix");
  }

  projection_matrix = glm::perspective(
    glm::radians(45.0f),
    ((float) window.value().get().width / (float) window.value().get().height),
    0.1f,
    100.0f
  );
}

void Camera::update() {
  // Update values with control input
  rotation += window.value().get().mouse.delta * window.value().get().dt * SENS;
  rotation.y = std::clamp(rotation.y, -1.57f, 1.57f);

  float cos_x = cos(rotation.x);
  float cos_y = cos(rotation.y);

  float sin_x = sin(rotation.x);
  float sin_y = sin(rotation.y);

  glm::vec3 forward = glm::vec3(
    sin_x,
    0.0f,
    cos_x
  );

  glm::vec3 right = glm::vec3(
    -cos_x,
    0.0f,
    sin_x
  );

  glm::vec3 direction = glm::vec3(
    cos_y * sin_x,
    sin_y,
    cos_y * cos_x
  );

  glm::vec3 up = glm::vec3(
    0.0f,
    1.0f,
    0.0f
  );

  glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);

  if (window.value().get().keyboard.keys[GLFW_KEY_W].down) {
    movement += forward;
  }
  if (window.value().get().keyboard.keys[GLFW_KEY_S].down) {
    movement -= forward;
  }
  if (window.value().get().keyboard.keys[GLFW_KEY_D].down) {
    movement += right;
  }
  if (window.value().get().keyboard.keys[GLFW_KEY_A].down) {
    movement -= right;
  }
  if (window.value().get().keyboard.keys[GLFW_KEY_SPACE].down) {
    movement += up;
  }
  if (window.value().get().keyboard.keys[GLFW_KEY_LEFT_SHIFT].down) {
    movement -= up;
  }

  // TODO: does this calculate sqrt twice?
  if (glm::length(movement) > 0.0f)
    position += glm::normalize(movement) * window.value().get().dt * SPEED;

  // Calculate new view matrix
  view_matrix = glm::lookAt(
    position,
    position + direction,
    up
  );
}

