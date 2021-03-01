#pragma once

#include "chunk.hh"

class World {
  public:
    Block& getAbsoluteBlock(int x, int y, int z);
    std::reference_wrapper<Chunk> getChunk(int x, int y);

    std::vector<std::reference_wrapper<Chunk>> chunks;
};
