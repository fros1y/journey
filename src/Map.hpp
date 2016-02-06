#ifndef _MAP_HPP
#define _MAP_HPP

#include "World.hpp"
#include <entityx/entityx.h>
#include <memory>
#include <string>
#include <vector>

struct Map {
  std::shared_ptr<World> world;
  int width;
  int height;

  std::vector<entityx::Entity> tiles;

  entityx::Entity get(int x, int y);
  void set(int x, int y, entityx::Entity e);
  void floodFill(int x, int y, std::function<void(int, int)> &builder);

  bool obstructs(int x, int y);
  bool populated(int x, int y);

  void toEntities();

  Map(std::shared_ptr<World> world, int width, int height)
      : world(world), width(width), height(height), tiles(width * height) {}

  void makeBlock(int x, int y);
  void makeFloor(int x, int y);

  void generateArena();
  void generateCavern();

  void addMonsters();
};
#endif
