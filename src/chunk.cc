#include "chunk.hh"

#include <noise1234.h>

#define NOISE_SCALE 0.1f

Chunk::Chunk(int x, int z)
  : x(x), z(z),
  blocks(CHUNK_HEIGHT, std::vector<std::vector<Block>>(CHUNK_SIZE, std::vector<Block>(CHUNK_SIZE, Block {0}))) {
  mesh = std::optional<std::reference_wrapper<ChunkMesh>> (*new ChunkMesh(*this));

  for (int x = 0; x < CHUNK_SIZE; x++) {
    for (int z = 0; z < CHUNK_SIZE; z++) {
      int height = (int) (1.0f + 
          (
            noise2(
              NOISE_SCALE * (x + (this->x * 16)),
              NOISE_SCALE * (z + (this->z * 16))
            ) + 1.0f
          ) * 5.0f);
      for (int y = 0; y < CHUNK_HEIGHT; y++) {
        if (y < height) {
          getBlock(x, y, z) = Block {0x2};
        } else if (y == height) {
          getBlock(x, y, z) = Block {0x1};
        } else {
          getBlock(x, y, z) = Block {0x0};
        }
      }
    }
  }

}

Block& Chunk::getBlock(int x, int y, int z) {
  return blocks.at(y).at(x).at(z);
}

