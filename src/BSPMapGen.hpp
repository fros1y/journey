//
// Created by Martin Galese on 2/21/16.
//

#ifndef JOURNEY_BSPMAPGEN_HPP
#define JOURNEY_BSPMAPGEN_HPP

#include "MapGen.hpp"
#include <libavoid/libavoid.h>

using Edge = std::pair<Position, Position>;

struct BSPMapGen : public MapGen, ITCODBspCallback {
  BSPMapGen(std::shared_ptr<World> world, const int width, const int height): MapGen(world, width, height), edges(), router(Avoid::OrthogonalRouting) {
    router.setRoutingOption(Avoid::improveHyperedgeRoutesMovingAddingAndDeletingJunctions, true);
    router.setRoutingOption(Avoid::nudgeSharedPathsWithCommonEndPoint, false);
//    router.setRoutingPenalty(Avoid::crossingPenalty, 10000);
//    router.setRoutingPenalty(Avoid::fixedSharedPathPenalty, 0);
//    router.setRoutingPenalty(Avoid::reverseDirectionPenalty, 100);
    router.setRoutingParameter(Avoid::shapeBufferDistance, 1);
  }
  void init();
  bool visitNode(TCODBsp *node, void *userData) override ;
  Position prevPos{-1,-1};

  std::vector<Edge> edges;
  Avoid::Router router;
};


#endif //JOURNEY_BSPMAPGEN_HPP
