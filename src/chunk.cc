#include "chunk.hh"

#include <noise1234.h>

#define NOISE_SCALE 0.05f
//#define PSYCH

Chunk::Chunk(int x, int z)
  : x(x), z(z),
  blocks(CHUNK_HEIGHT, std::vector<std::vector<Block>>(CHUNK_SIZE, std::vector<Block>(CHUNK_SIZE, Block {0}))) {
  mesh = std::optional<std::reference_wrapper<ChunkMesh>> (*new ChunkMesh(*this));

#ifdef PSYCH
  for (int x = 0; x < CHUNK_SIZE; x++) {
    for (int z = 0; z < CHUNK_SIZE; z++) {
      for (int y = 0; y < CHUNK_HEIGHT; y++) {
        int v = 1 + noise3(
            NOISE_SCALE * (x + (this->x * 16)),
            NOISE_SCALE * y,
            NOISE_SCALE * (z + (this->z * 16))
        );
        if (v > 0.8) {
          getBlock(x, y, z) = Block {0x1};
        } else if (v > 0.7) {
          getBlock(x, y, z) = Block {0x2};
        } else if (v > 0.6) {
          getBlock(x, y, z) = Block {0x3};
        } else {
          getBlock(x, y, z) = Block {0x0};
        }

      }
    }
  }
#else
  for (int x = 0; x < CHUNK_SIZE; x++) {
    for (int z = 0; z < CHUNK_SIZE; z++) {
      int height = (int) (20.0f +
          (
            noise2(
              NOISE_SCALE * (x + (this->x * 16)),
              NOISE_SCALE * (z + (this->z * 16))
            ) + 1.0f
          ) * 15.0f);
      for (int y = 0; y < CHUNK_HEIGHT; y++) {
        if (y < height) {
          if (y < (height-2)) {
            getBlock(x, y, z) = Block {0x3};
          } else {
            getBlock(x, y, z) = Block {0x2};
          }
        } else if (y == height) {
          getBlock(x, y, z) = Block {0x1};
        } else {
          getBlock(x, y, z) = Block {0x0};
        }
      }
    }
  }
#endif
}

Block& Chunk::getBlock(int x, int y, int z) {
  return blocks.at(y).at(x).at(z);
}

