#pragma once

#include "gfx.hh"

class VBO {
  public:
    VBO(GLint type, bool dynamic);
    ~VBO();
    void init();
    void bind();
    void write(void *data, size_t offset, size_t count);

  private:
    GLuint handle;
    GLint type;
    bool dynamic;
};
