#include <stdio.h>

#include "window.hh"
#include "renderer.hh"

int main() {
  Window window(1024, 720, "C++ Game Window");
  Renderer renderer(&window);
  window.setRenderer(&renderer);
  window.start();
}
