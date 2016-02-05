#ifndef _MAP_HPP
#define _MAP_HPP

#include "World.hpp"
#include <memory>
#include <string>

struct Map {
  std::shared_ptr<World> world;
  int width;
  int height;

  Map(std::shared_ptr<World> world, int width, int height)
      : world(world), width(width), height(height) {}

  void makeBlock(int x, int y);
  void generateArena();
};
#endif
