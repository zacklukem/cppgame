#include "world.hh"

inline int modulo(int x,int N){
    return (x % N + N) %N;
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
  int bx = (int) modulo(x, CHUNK_SIZE);
  int bz = (int) modulo(z, CHUNK_SIZE);

  try {
    return getChunk(cx, cz).get().getBlock(bx, y, bz);
  } catch {
    return *new Block {0x0};
  }
}

