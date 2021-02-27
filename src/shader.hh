#pragma once

#include "gfx.hh"
#include <string>

class Shader {
  public:
    ~Shader();
    void load(const char* vertex_path, const char* frag_path);
    GLint getUniform(const char* name);
    void use();
  private:
    GLuint id = 0;
};
