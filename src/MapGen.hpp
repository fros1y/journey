//
// Created by Martin Galese on 2/16/16.
//

#ifndef JOURNEY_MAPGEN_HPP
#define JOURNEY_MAPGEN_HPP

#include "components.hpp"
#include "world.hpp"
#include <vector>

enum class Element { Empty, Hall, Room, Floor, Wall, Rock, Door };
enum class Direction {Up, Down, Left, Right, Count};

struct Room {
  Position center;
  int width;
  int height;
  Room(const Position center, const int width, const int height) : center(center), width(width), height(height) {}
};

struct MapGen {
  std::shared_ptr<World> world;
  int width, height;
  std::vector<std::vector<Element>> map;

  Position findFree() {
    int x, y;
    do {
      x = world->rnd->getInt(1, width-1);
      y = world->rnd->getInt(1, height-1);
    } while(map[x][y]!=Element::Floor);
    return Position(x,y);
  }

  MapGen(std::shared_ptr<World> world, const int width, const int height) : world(world), width(width), height(height) {
    map.resize(width, std::vector<Element>(height, Element::Rock));
  }

  virtual void init() = 0;

  template<typename F>
  void forAll(F function) {
    for(auto i = 0; i < width; i++) {
      for(auto j = 0; j < height; j++) {
        function(map[i][j], i, j);
      }
    }
  }

  bool ellipseFill(int leftMost,
                   int rightMost,
                   int topMost,
                   int bottomMost,
                   Element fill, bool squash=false);

  bool rectFill(const Position &center, const int width, const int height, const Element fill, const bool squash=false);

  bool rectFill(const int leftMostIn,
                const int rightMostIn,
                const int topMostIn,
                const int bottomMostIn,
                const Element fill, const bool squash);

  void floodFill(const int x, const int y, const Element fill);

  bool buildRoom(const int x, const int y, int width, int height, bool squash=false) {
    return buildRoom(Position(x, y), width, height, squash);
  }

  bool buildRoom(const Room& r, bool squash=false);

  bool buildRoom(const Position& center, int width, int height, bool squash=false) {
    auto room = Room(center, width, height);
    return buildRoom(room, squash);
  }

  bool buildCorridor(const Room& r1, const Room& r2);
  bool buildCorridor(const Position& p1, const Position& p2);
};


#endif //JOURNEY_MAPGEN_HPP
