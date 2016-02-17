//
// Created by Martin Galese on 2/13/16.
//

#ifndef JOURNEY_GRAPHMAPGEN_HPP
#define JOURNEY_GRAPHMAPGEN_HPP

#include <vector>
#include "World.hpp"
#include <libavoid/libavoid.h>
#include "MapGen.hpp"

void _libavoid_callback(void *ptr);

struct GraphMapGen : public MapGen {
  GraphMapGen(std::shared_ptr<World> world, const int width, const int height): MapGen(world, width, height) {}

  void init();

//  bool buildCorridor(const int x, const int y, const Direction grow, const bool deadend=true);
//  Direction spaceAvailable(const int x, const int y);
//  Direction findSpace(int &x, int &y, const int maxTries);
};


#endif //JOURNEY_MAPGEN_HPP
