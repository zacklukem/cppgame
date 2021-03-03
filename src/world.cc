#include "world.hh"

#define MODULO(x, N) ((x % N + N) % N)

World::World() {
  for (int x = -5; x <= 5; x++) {
    for (int y = -5; y <= 5; y++) {
      chunks.push_back(*new Chunk(x, y));
    }
  }
}

std::reference_wrapper<Chunk> World::getChunk(int x, int z) {
  for (auto chunk : chunks) {
    if (chunk.get().x == x && chunk.get().z == z) {
      return chunk;
    }
  }
  throw std::runtime_error("Chunk access out of bounds");
}

Block& World::getAbsoluteBlock(int x, int y, int z) {
  int cx = (int) floor((float) x / (float) CHUNK_SIZE);
  int cz = (int) floor((float) z / (float) CHUNK_SIZE);
  int bx = (int) MODULO(x, CHUNK_SIZE);
  int bz = (int) MODULO(z, CHUNK_SIZE);

  try {
    return getChunk(cx, cz).get().getBlock(bx, y, bz);
  } catch (std::exception& e) {
    return *new Block {0x0};
  }
}

