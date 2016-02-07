#ifndef _MAP_HPP
#define _MAP_HPP

#include "DjikstraMap.hpp"
#include "World.hpp"
#include <entityx/entityx.h>
#include <libtcod.hpp>
#include <memory>
#include <string>
#include <vector>

struct Map {
  std::shared_ptr<World> world;
  int width;
  int height;

  int calculatedForTurn;

  std::shared_ptr<TCODMap> tcod_map;
  std::shared_ptr<DjikstraMap> d_map;

  std::vector<entityx::Entity> tiles;

  entityx::Entity get(int x, int y);
  void set(int x, int y, entityx::Entity e);
  void floodFill(int x, int y, std::function<void(int, int)> &builder);

  bool obstructs(int x, int y);
  bool populated(int x, int y);

  Map(std::shared_ptr<World> world, int width, int height)
      : world(world), width(width), height(height), calculatedForTurn(-1), tiles(width * height) {}

  void makeBlock(int x, int y);
  void makeFloor(int x, int y);

  void generateArena();
  void generateCavern();

  void addMonsters();

  void calculateMaps();

  void computeMovesTo(int d_x, int d_y);
  void nextStepFrom(int x, int y, int &n_x, int &n_y);
  bool canReachFrom(int x, int y);

  void computeFoVFrom(int x, int y, int range);
  bool isInFoV(int x, int y);
};
#endif
