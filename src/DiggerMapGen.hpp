//
// Created by Martin Galese on 2/16/16.
//

#ifndef JOURNEY_DIGGERMAPGEN_HPP
#define JOURNEY_DIGGERMAPGEN_HPP

#include "MapGen.hpp"

struct Digger;

struct DiggerMapGen: public MapGen {
  std::vector<std::shared_ptr<Digger>> diggers;

  DiggerMapGen(std::shared_ptr<World> world, const int width, const int height): MapGen(world, width, height) {}
  void init();
};

struct Digger {
  DiggerMapGen *mapgen;
  Position position;
  Direction direction;
  int turnsSinceRoom = 0;
  int turnsSinceDirection = 0;
  int maxTurnsSinceDirection = 2;
  int maxTurnsSinceRoom = 20;
  bool alive = true;
  void dig(const int turnCount);

  Digger(DiggerMapGen *mapgen, const int x, const int y): mapgen(mapgen) {
    position.x = x;
    position.y = y;
    direction = Direction(mapgen->world->rnd->getInt(0, (int)Direction::Count-1));
  }

  Digger(DiggerMapGen *mapgen,
         const Position &position,
         const Direction &direction) : mapgen(mapgen),
                                       position(position),
                                       direction(direction) {}

  Digger(DiggerMapGen *mapgen) : mapgen(mapgen) {
    position.x = mapgen->world->rnd->getInt(0, mapgen->width-1);
    position.y = mapgen->world->rnd->getInt(0, mapgen->height-1);
    direction = Direction(mapgen->world->rnd->getInt(0, (int)Direction::Count-1));
  }

};


#endif //JOURNEY_DIGGERMAPGEN_HPP
