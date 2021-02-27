#include "vbo.hh"

#include "gfx.hh"

VBO::VBO(GLint _type, bool _dynamic) {
  type = _type;
  dynamic = _dynamic;
}


VBO::~VBO() {
  glDeleteBuffers(1, &handle);
}

void VBO::init() {
  glGenBuffers(1, &handle);
}

void VBO::bind() {
  glBindBuffer(type, handle);
}

void VBO::write(void *data, size_t offset, size_t count) {
  bind();
  glBufferData(type, count - offset, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}
