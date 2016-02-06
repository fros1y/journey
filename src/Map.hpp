#ifndef _MAP_HPP
#define _MAP_HPP

#include "World.hpp"
#include <memory>
#include <string>
#include <vector>

struct Map {
  std::shared_ptr<World> world;
  int width;
  int height;
  std::vector<char> tiles;

  char get(int x, int y);
  void set(int x, int y, char v);
  void floodFill(int x, int y, char v);

  void toEntities();

  Map(std::shared_ptr<World> world, int width, int height)
      : world(world), width(width), height(height), tiles(width * height, ' ') {
  }

  void makeBlock(int x, int y);
  void makeFloor(int x, int y);

  void generateArena();
  void generateCavern();

  void addMonsters();
};
#endif
