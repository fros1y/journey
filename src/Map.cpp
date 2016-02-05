#include "Map.hpp"

void Map::makeBlock(int x, int y) {
  auto block = world->entities.create();
  block.assign<Position>(x, y);
  block.assign<Obstruction>();
  block.assign<Render>('#');
}

void Map::generateArena() {
  for (auto i = 0; i < height; i++) {
    makeBlock(i, 0);
    makeBlock(i, width - 1);
  }
  for (auto i = 1; i < width - 1; i++) {
    makeBlock(0, i);
    makeBlock(height - 1, i);
  }
}
