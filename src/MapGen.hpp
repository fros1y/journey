//
// Created by Martin Galese on 2/16/16.
//

#ifndef JOURNEY_MAPGEN_HPP
#define JOURNEY_MAPGEN_HPP

#include "components.hpp"
#include "world.hpp"
#include <vector>
#include <libavoid/libavoid.h>

enum class Element { Empty, Floor, Wall, Rock, Door };
enum class Direction {Up, Down, Left, Right, Count};
using Edge = std::pair<Position, Position>;


struct Room {
  Position center;
  int width;
  int height;
  Room(const Position center, const int width, const int height) : center(center), width(width), height(height) {}
};

struct MapGen: public ITCODBspCallback {
  std::shared_ptr<World> world;
  int width, height;
  std::vector<std::vector<Element>> map;
  Position prevPos{-1, -1};
  std::vector<Edge> edges;
  Avoid::Router router;


  Position findFree() {
    int x, y;
    do {
      x = world->rnd->getInt(1, width-1);
      y = world->rnd->getInt(1, height-1);
    } while(map[x][y]!=Element::Floor);
    return Position(x,y);
  }

  MapGen(std::shared_ptr<World> world, const int width, const int height):  world(world), width(width), height(height), edges(), router(Avoid::OrthogonalRouting) {
    map.resize(width, std::vector<Element>(height, Element::Rock));
    router.setRoutingOption(Avoid::improveHyperedgeRoutesMovingAddingAndDeletingJunctions, true);
    router.setRoutingOption(Avoid::nudgeSharedPathsWithCommonEndPoint, false);
    router.setRoutingPenalty(Avoid::crossingPenalty, 10000);
    router.setRoutingPenalty(Avoid::fixedSharedPathPenalty, 0);
    router.setRoutingPenalty(Avoid::reverseDirectionPenalty, 100);
    router.setRoutingParameter(Avoid::shapeBufferDistance, 1);
  }


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

  bool rectFill(const Position &center,
                const int width,
                const int height,
                const Element fill,
                const bool squash=false);

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

  void init();

  bool visitNode(TCODBsp *node, void *userData) override ;
};


#endif //JOURNEY_MAPGEN_HPP
