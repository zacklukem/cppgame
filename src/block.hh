#pragma once

#include <glm/glm.hpp>

// +y 010
#define UP 0
// -y -010
#define DOWN 1
// +z 001
#define NORTH 2
// -z -001
#define SOUTH 3
// -x -100
#define EAST 4
// +x 100
#define WEST 5

struct Block {
  std::uint16_t id;
  // std::uint16_t padding;
};

glm::vec2* getUVCoords(Block& block, unsigned int face);

