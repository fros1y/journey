//
// Created by Martin Galese on 2/13/16.
//

#ifndef JOURNEY_MAPGEN_HPP
#define JOURNEY_MAPGEN_HPP

#include <vector>
#include "World.hpp"
enum class Element { Empty, Floor, Wall, Rock };
enum class Direction {None, Up, Down, Left, Right};


struct Room {
  Position center;
  int width;
  int height;
  Room(const Position center, const int width, const int height) : center(center), width(width), height(height) {}
};

struct Corridor {
  int firstRoom;
  int secondRoom;
  Corridor(const int firstRoom, const int secondRoom) : firstRoom(firstRoom), secondRoom(secondRoom) {}
};


struct MapGen {
  std::shared_ptr<World> world;
  int width, height;
  std::vector<std::vector<Element>> map;


  MapGen(std::shared_ptr<World> world, const int width, const int height) : world(world), width(width), height(height) {
    map.resize(width, std::vector<Element>(height, Element::Rock));
  }

  void init();

  template<typename F>
  void forAll(F function) {
    for(auto i = 0; i < width; i++) {
      for(auto j = 0; j < height; j++) {
        function(map[i][j], i, j);
      }
    }
  }

 bool rectFill(
      int ,
      int ,
      int ,
      int ,
      Element);

  void floodFill(const int x, const int y, const Element fill);

  bool buildRoom(const Room& r);
  bool buildCorridor(const Room& r1, const Room& r2);

//  bool buildCorridor(const int x, const int y, const Direction grow, const bool deadend=true);
//  Direction spaceAvailable(const int x, const int y);
//  Direction findSpace(int &x, int &y, const int maxTries);
};


#endif //JOURNEY_MAPGEN_HPP
