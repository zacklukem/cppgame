#pragma once

#include "vbo.hh"
#include "gfx.hh"

class VAO {
  public:
    ~VAO();
    void init();
    void bind();
    void attr(VBO& vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset);
  private:
    GLuint handle = 0;
};
