#include "window.hh"

#include <stdexcept>
#include "renderer.hh"

#include "gfx.hh"
#include "chunk.hh"

// TODO: kill me. NOW
static Window* window_g;

static void size_callback(GLFWwindow *handle, int width, int height) {
    glViewport(0, 0, width, height);
    window_g->width = width;
    window_g->height = height;
    window_g->camera->calculateProjectionMatrix();
}

static void mouse_callback(GLFWwindow *handle, int button, int action, int mods) {
  if (button < 0) {
    return;
  }

  switch (action) {
    case GLFW_PRESS:
      printf("Mouse press\n");
      window_g->camera->castRay(button);
      break;
    case GLFW_RELEASE:
      break;
    default:
      break;
  }
}

Window::Window(int _width, int _height, const char* title)
  : camera(new Camera()),
  world(*new World()),
  mouse(*new Mouse {glm::vec2(0.0f), glm::vec2(0.0f)}),
  keyboard(*new Keyboard) {
  width = _width;
  height = _height;

  if (!glfwInit()) {
    throw std::runtime_error("Failed to load GLFW");
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
  glfwMakeContextCurrent(glfw_window);

  window_g = this;

  if (glfw_window == NULL) {
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLFW window");
  }

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    throw std::runtime_error("Failed to load OpenGL context");
  }

}

void Window::start() {

  glfwSetMouseButtonCallback(glfw_window, mouse_callback);
  glfwSetFramebufferSizeCallback(glfw_window, size_callback);

  camera->setWindow(*this);
  camera->calculateProjectionMatrix();

  set_cursor_grabbed(*this, true);
  init_inputs(*this);

  if (renderer == NULL) {
    throw std::runtime_error("Renderer must be initialized before starting the graphics loop");
  }

  glViewport(0, 0, width, height);
  renderer->init();

  while (!glfwWindowShouldClose(glfw_window)) {
    auto start_time = glfwGetTime();

    // 54,199,242
    // glClearColor(0.21f, 0.78f, 0.94f, 1.0f);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    camera->update();

    // render
    renderer->render();

    update_cursor(*this);

    glfwSwapBuffers(glfw_window);
    glfwPollEvents();

    dt = (float) (glfwGetTime() - start_time);
  }

}

void Window::setRenderer(Renderer* _renderer) {
  renderer = _renderer;
}

Window::~Window() {
  glfwTerminate();
}

