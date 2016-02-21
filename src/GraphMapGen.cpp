//
// Created by Martin Galese on 2/13/16.
//

#include <libtcod.hpp>
#include "GraphMapGen.hpp"
#include "utils.h"
#include <boost/range/irange.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/make_connected.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/make_biconnected_planar.hpp>


#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/point_traits.hpp>
#include <boost/random/linear_congruential.hpp>
#include <iostream>
#include <boost/limits.hpp>
#include <fstream>
#include <string>

#include <libavoid/libavoid.h>

std::vector<Position> _libavoid_storage;

Position getRandomDoorLocation(const Room& r, const World& world) {
  auto leftMost = int(r.center.x - r.width / 2.0);
  auto rightMost = int(r.center.x + r.width / 2.0);
  auto topMost = int(r.center.y - r.height / 2.0);
  auto bottomMost = int(r.center.y + r.height / 2.0);
  auto d = Direction(world.rnd->getInt(0, int(Direction::Count) - 1));

  int x = 0;
  int y = 0;

  printf("D: %i\n", d);

  switch (d) {
    case Direction::Up:
      y = topMost - 1;
      x = world.rnd->getInt(leftMost + 2, rightMost - 2);
      break;
    case Direction::Down:
      y = bottomMost;
      x = world.rnd->getInt(leftMost + 2, rightMost - 2);
      break;
    case Direction::Left:
      y = world.rnd->getInt(topMost + 2, bottomMost - 2);
      x = leftMost - 1;
      break;
    case Direction::Right:
      y = world.rnd->getInt(topMost + 2, bottomMost - 2);
      x = rightMost;
      break;
    default:
      break;
  }
  return Position(x, y);
}

void _libavoid_callback(void *ptr) {
      Avoid::ConnRef *connRef = (Avoid::ConnRef *) ptr;
      const Avoid::PolyLine& route = connRef->route();
      for(auto i = 0; i < route.ps.size()-1; ++i) {
        int x = int(route.ps[i].x);
        int y = int(route.ps[i].y);
        TCODLine::init(x, y, int(route.ps[i+1].x), int(route.ps[i+1].y));
        do {
          _libavoid_storage.emplace_back(x, y);
        } while(!TCODLine::step(&x, &y));
      }
    }

void GraphMapGen::init() {
  _libavoid_storage.clear();

  auto router = std::make_unique<Avoid::Router>(Avoid::OrthogonalRouting);
  router->setRoutingOption(Avoid::improveHyperedgeRoutesMovingAddingAndDeletingJunctions, true);
  router->setRoutingOption(Avoid::nudgeSharedPathsWithCommonEndPoint, false);
  router->setRoutingPenalty(Avoid::crossingPenalty, 0);
  router->setRoutingPenalty(Avoid::fixedSharedPathPenalty, 10000);
  router->setRoutingPenalty(Avoid::reverseDirectionPenalty, 0);
  router->setRoutingParameter(Avoid::shapeBufferDistance, 3);

  bool playerRoom = true;
  int roomCount = 10;
  std::vector<std::shared_ptr<Room>> rooms(roomCount, nullptr);

  for (auto i : boost::irange(0, roomCount)) {
    int x, y;

    auto w = world->rnd->getInt(4, 20);
    auto h = world->rnd->getInt(4, w, w * 0.625);

    if (playerRoom) {
      x = world->currLevel->width / 2;
      y = world->currLevel->height / 2;
      playerRoom = false;
    } else {
      x = world->rnd->getInt(w + 1, width - w);
      y = world->rnd->getInt(h + 1, height - h);
    }
    auto room = std::make_shared<Room>(Position(x, y), w, h);

    if (buildRoom(*room)) {
      rooms[i] = room;
      Avoid::Rectangle roomRect(Avoid::Point(x, y), w, h);
      Avoid::ShapeRef *roomShape = new Avoid::ShapeRef(router.get(), roomRect);
    } else {
      rooms[i] = nullptr;
    }
  }

  for(auto i : boost::irange(0, 15)) {
    int srcN, destN;
    do {
      srcN = world->rnd->getInt(0, roomCount-1);
      destN = world->rnd->getInt(0, roomCount-1);
    } while(srcN == destN || !rooms[srcN] || !rooms[destN]);

    auto src = rooms[srcN];
    auto dest = rooms[destN];
//    auto srcP = src->center;
//    auto destP = dest->center;
    auto srcP = getRandomDoorLocation(*src, *world);
    auto destP = getRandomDoorLocation(*dest, *world);
    map[srcP.x][srcP.y] = Element::Door;
    map[destP.x][destP.y] = Element::Door;


    Avoid::Point srcPt(srcP.x, srcP.y);
    Avoid::Point destPt(destP.x, destP.y);
    auto connRef = new Avoid::ConnRef(router.get(), srcPt, destPt);
    connRef->setCallback(&_libavoid_callback, connRef);
  }

  router->processTransaction();

  for(auto p : _libavoid_storage) {
    map[clamp(p.x, 1, width-2)][clamp(p.y, 1, width-2)] = Element::Floor;
  }
}
