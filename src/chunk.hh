#pragma once

class Chunk;

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256

#include <vector>
#include <optional>
#include <functional>

#include "chunkmesh.hh"
#include "block.hh"

class Chunk {
  public:
    Chunk(int x, int z);
    ~Chunk();
    Block& getBlock(int x, int y, int z);
    std::optional<std::reference_wrapper<ChunkMesh>> mesh = std::nullopt;
    std::vector<std::vector<std::vector<Block>>> blocks;
    int x, z;
};
