#include "block.hh"

const int blocks[][6][2] = {
  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
  {{1,0},{2,0},{0,0},{0,0},{0,0},{0,0}},
  {{2,0},{2,0},{2,0},{2,0},{2,0},{2,0}},
  {{3,0},{3,0},{3,0},{3,0},{3,0},{3,0}},
  {{4,0},{4,0},{4,0},{4,0},{4,0},{4,0}},
  {{6,0},{6,0},{5,0},{5,0},{5,0},{5,0}},
  {{7,0},{7,0},{7,0},{7,0},{7,0},{7,0}},
  {{8,0},{8,0},{8,0},{8,0},{8,0},{8,0}},
  {{9,0},{9,0},{9,0},{9,0},{9,0},{9,0}},
};

#define BF 0.99f

glm::vec2* getUVCoords(Block& block, unsigned int face) {
  glm::vec2* coords = new glm::vec2[4];

  int bx = blocks[block.id][face][0];
  int by = blocks[block.id][face][1];

  switch (face) {
    case NORTH:
    case SOUTH:
      coords[0] = glm::vec2(bx + BF, by + BF);
      coords[1] = glm::vec2(bx + 0, by + BF);
      coords[2] = glm::vec2(bx + 0, by + 0);
      coords[3] = glm::vec2(bx + BF, by + 0);
      break;
    case EAST:
    case WEST:
      coords[0] = glm::vec2(bx + BF, by + BF);
      coords[1] = glm::vec2(bx + BF, by + 0);
      coords[2] = glm::vec2(bx + 0, by + 0);
      coords[3] = glm::vec2(bx + 0, by + BF);
      break;
    default:
      coords[0] = glm::vec2(bx + 0, by + 0);
      coords[1] = glm::vec2(bx + BF, by + 0);
      coords[2] = glm::vec2(bx + BF, by + BF);
      coords[3] = glm::vec2(bx + 0, by + BF);
      break;
  }


  for (int i = 0; i < 4; i++) {
    coords[i] /= 32.0f;
    coords[i].y = 32.0f - coords[i].y;
  }

  return coords;
}


