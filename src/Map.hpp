#ifndef _MAP_HPP
#define _MAP_HPP

#include <entityx/entityx.h>
#include <libtcod.hpp>
#include <memory>
#include <string>
#include <vector>
#include "DjikstraMap.hpp"
#include "LightMap.hpp"
#include "World.hpp"
#include "components.hpp"

struct Map {
  std::shared_ptr<World> world;
  int width;
  int height;

  int calculatedForTurn;

  std::shared_ptr<TCODMap> tcod_map;
  std::shared_ptr<DjikstraMap> d_map;
  std::shared_ptr<LightMap> l_map;

  std::unordered_multimap<Position, std::shared_ptr< entityx::Entity > > entities;

  std::vector<entityx::Entity> tiles;

  entityx::Entity get(int x, int y);
  void set(int x, int y, entityx::Entity e);
  void floodFill(int x, int y, std::function<void(int, int)> &builder);

  bool populated(int x, int y);

  Map(std::shared_ptr<World> world, int width, int height)
      : world(world),
        width(width),
        height(height),
        calculatedForTurn(-1),
        tiles(width * height) { }

  void makeBlock(int x, int y);
  void makeFloor(int x, int y);
  void makeLightSource(int x, int y, float brightness = 1, TCODColor color = TCODColor::yellow);

  void generateArena();

  void addMonsters();

  void calculateMaps();

  void computeMovesTo(int d_x, int d_y);
  void nextStepFrom(int x, int y, int &n_x, int &n_y);
  bool canReachFrom(int x, int y);

  void computeFoVFrom(int x, int y, int range);
  bool isInFoV(int x, int y);
  void calculateLighting() const;
};
#endif
