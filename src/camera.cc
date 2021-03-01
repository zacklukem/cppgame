#include "camera.hh"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <functional>
#include <stdio.h>
#include <limits>
#include <algorithm>
#include "chunk.hh"
#include <math.h>

#define SENS 0.1f
#define SPEED 8.0f

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

#define MAX_DISTANCE 10
#define SIGN(num) signbit(num) ? -1 : 1

#define MODULO(x, N) ((x % N + N) % N)


void Camera::castRay() {
  glm::vec3 direction = glm::vec3(
    cos(rotation.y) * sin(rotation.x),
    sin(rotation.y),
    cos(rotation.y) * cos(rotation.x)
  );

  glm::ivec3 p = glm::ivec3(floor(position.x), floor(position.y), floor(position.z));
  glm::ivec3 step = glm::ivec3(SIGN(direction.x), SIGN(direction.y), SIGN(direction.z));
  glm::vec3 tmax = glm::vec3(
      (direction.x > 0 ? (ceil(position.x) - position.x) : (position.x - floor(position.x))) / abs(direction.x),
      (direction.y > 0 ? (ceil(position.y) - position.y) : (position.y - floor(position.y))) / abs(direction.y),
      (direction.z > 0 ? (ceil(position.z) - position.z) : (position.z - floor(position.z))) / abs(direction.z)
  );

  glm::vec3 tdelta = glm::vec3(step) / direction;
  // TODO: direction should already be normal
  float radius = (float) MAX_DISTANCE / glm::length(direction);

  bool has_intersection = false;
  glm::ivec3 location;

  while (true) {
    if (window.value().get().renderer->getAbsoluteBlock(p.x, p.y, p.z).id) {
      location = p;
      has_intersection = true;
      break;
    }

    if (tmax.x < tmax.y) {
      if (tmax.x < tmax.z) {
        if (tmax.x > radius) {
          break;
        }

        p.x += step.x;
        tmax.x += tdelta.x;
        // direction?
      } else {
        if (tmax.z > radius) {
          break;
        }

        p.z += step.z;
        tmax.z += tdelta.z;
        // dir?
      }
    } else {
      if (tmax.y < tmax.z) {
        if (tmax.y > radius) {
          break;
        }

        p.y += step.y;
        tmax.y += tdelta.y;
        // dir?
      } else {
        if (tmax.z > radius) {
          break;
        }
        p.z += step.z;
        tmax.z += tdelta.z;
        //dir?
      }
    }
  }

  if (has_intersection) {
    printf(" INTERSECTION: %d, %d, %d\n", location.x, location.y, location.z);
    printf("          POS: %d, %d, %d\n", (int) floor(position.x), (int) floor(position.y), (int) floor(position.z));
    printf("       FACING: %d, %d, %d\n", step.x, step.y, step.z);
    // printf("        PLANE: %s\n", plane == 0 ? "x" : plane == 1 ? "y" : plane == 2 ? "z" : "u suck");

    int cx = (int) floor((float) location.x / (float) CHUNK_SIZE);
    int cz = (int) floor((float) location.z / (float) CHUNK_SIZE);
    int bx = (int) MODULO(location.x, CHUNK_SIZE);
    int bz = (int) MODULO(location.z, CHUNK_SIZE);
    window.value().get().renderer->getAbsoluteBlock(location.x, location.y, location.z) = Block {0x0};
    window.value().get().renderer->getChunk(cx, cz).get().mesh.value().get().gen(window.value());
    if (bx == 0) {
      window.value().get().renderer->getChunk(cx-1, cz).get().mesh.value().get().gen(window.value());
    } else if (bx == CHUNK_SIZE - 1) {
      window.value().get().renderer->getChunk(cx+1, cz).get().mesh.value().get().gen(window.value());
    }
    if (bz == 0) {
      window.value().get().renderer->getChunk(cx, cz-1).get().mesh.value().get().gen(window.value());
    } else if (bz == CHUNK_SIZE - 1) {
      window.value().get().renderer->getChunk(cx, cz+1).get().mesh.value().get().gen(window.value());
    }
  } else {
    printf("NO INTERSECTION\n");
  }
}

