#include "image_util.hh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>

#include "gfx.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GLuint load_image(const char* path) {
  int width, height, channels;
  printf("Loading image: %s\n", path);
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
  printf("Loaded: %s; w=%d, h=%d, c=%d\n", path, width, height, channels);


  if (data == NULL) {
    throw std::runtime_error("Failed to load image");
  }

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(
      GL_TEXTURE_2D,
      0,
      channels == 4 ? GL_RGBA8 : GL_RGB,
      width, height, 0,
      channels == 4 ? GL_RGBA : GL_BGR,
      GL_UNSIGNED_BYTE, data);

	// Poor filtering, or ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

  return textureID;
}

