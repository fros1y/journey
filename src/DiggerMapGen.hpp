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
  bool alive = true;

  virtual void dig(const int turnCount) = 0;


  void place(Element e) {
    mapgen->map[position.x][position.y] = e;
  }

  bool outOfBounds(const int buffer = 1) {
    return (position.x < buffer || position.y < buffer || position.x >= mapgen->width - buffer || position.y >= mapgen->height - buffer);
  }

  void stepDirection(const int stepSize = 1) {
    switch(direction) {
      case Direction::Up:
        position.y -= stepSize;
        break;
      case Direction::Down:
        position.y += stepSize;
        break;
      case Direction::Left:
        position.x -= stepSize;
        break;
      case Direction::Right:
        position.x += stepSize;
        break;
      default:
        break;
    }
  }

  Digger(DiggerMapGen *mapgen,
         const Position &position,
         const Direction &direction) : mapgen(mapgen),
                                       position(position),
                                       direction(direction) {}

  Digger(DiggerMapGen *mapgen, const int x, const int y): mapgen(mapgen) {
    position.x = x;
    position.y = y;
    direction = Direction(mapgen->world->rnd->getInt(0, (int)Direction::Count-1));
  }

  Digger(DiggerMapGen *mapgen) : mapgen(mapgen) {
    position.x = mapgen->world->rnd->getInt(0, mapgen->width-1);
    position.y = mapgen->world->rnd->getInt(0, mapgen->height-1);
    direction = Direction(mapgen->world->rnd->getInt(0, (int)Direction::Count-1));
  }
};

struct CorridorDigger : Digger {

  int buffer = 10;
  int turnsSinceDirection = 0;
  int maxTurnsSinceDirection = 13;
  int minTurnsSinceDirection = 7;
  double turnOdds = 0.1;

  int footprint = 2;

  void dig(const int turnCount) override;

  CorridorDigger(DiggerMapGen *mapgen) : Digger(mapgen) {}
  CorridorDigger(DiggerMapGen *mapgen, const int x, const int y) : Digger(mapgen, x, y) {}
  CorridorDigger(DiggerMapGen *mapgen, const Position &position,
                 const Direction &direction) : Digger(mapgen, position, direction) {}
};


#endif //JOURNEY_DIGGERMAPGEN_HPP
