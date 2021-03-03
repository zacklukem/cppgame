#pragma once

class World;

#include "chunk.hh"
#include "block.hh"

class World {
  public:
    World();
    Block& getAbsoluteBlock(int x, int y, int z);
    std::reference_wrapper<Chunk> getChunk(int x, int y);
    std::vector<std::reference_wrapper<Chunk>> chunks;
};
