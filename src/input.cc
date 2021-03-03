#include "input.hh"

#include <algorithm>

static Window* window_g;

void set_cursor_grabbed(Window& window, bool grabbed) {
  glfwSetInputMode(window.glfw_window, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

  glm::vec2 pos = glm::vec2(xpos, ypos);

  // clamp for max speed
  window_g->mouse.delta = pos - window_g->mouse.position;
  window_g->mouse.delta.x = -std::clamp(window_g->mouse.delta.x, -100.0f, 100.0f);
  window_g->mouse.delta.y = -std::clamp(window_g->mouse.delta.y, -100.0f, 100.0f);

  window_g->mouse.position = pos;
}

static void key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods) {
  if (key < 0) {
    return;
  }
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    set_cursor_grabbed(*window_g, false);
  }

  switch (action) {
    case GLFW_PRESS:
      if (key < GLFW_KEY_9 && key > GLFW_KEY_0) {
        window_g->selected = key - GLFW_KEY_0;

      }
      window_g->keyboard.keys[key].down = true;

      break;
    case GLFW_RELEASE:
      window_g->keyboard.keys[key].down = false;
      break;
    default:
      break;
  }
}

void init_inputs(Window& window) {
  window_g = &window;

  for (int i = 0; i < GLFW_KEY_LAST; i++) {
    window.keyboard.keys[i] = (Button) {false, false, false, false, false};
  }

  glfwSetCursorPosCallback(window.glfw_window, cursor_position_callback);
  glfwSetKeyCallback(window.glfw_window, key_callback);
}

void update_cursor(Window& window) {
  window.mouse.delta = glm::vec2(0.0f);
}
