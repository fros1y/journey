//
// Created by Martin Galese on 2/21/16.
//


#include <boost/range/irange.hpp>
#include "BSPMapGen.hpp"
#include "utils.h"

extern std::vector<Position> _libavoid_storage;
void _libavoid_callback(void *ptr);

void BSPMapGen::init() {
  _libavoid_storage.clear();
  auto BSP = std::make_unique<TCODBsp>(5,5, width-5, height-5);
  BSP->splitRecursive(world->rnd, 8, 20, 20, 1000.0f, 1000.0f);
  BSP->traverseInOrder(this, NULL);
  for(auto edge : edges) {
    auto srcP = edge.first;
    auto destP = edge.second;
    Avoid::Point srcPt(srcP.x, srcP.y);
    Avoid::Point destPt(destP.x, destP.y);
    auto connRef = new Avoid::ConnRef(&router, srcPt, destPt);
    connRef->setCallback(&_libavoid_callback, connRef);
  }
  router.processTransaction();
  for(auto p : _libavoid_storage) {
    map[clamp(p.x, 1, width - 2)][clamp(p.y, 1, width - 2)] = Element::Floor;
  }
  world->currLevel->playerStart = findFree();
}

bool BSPMapGen::visitNode(TCODBsp *node, void *userData) {
  if(node->isLeaf()) {
    auto buffer = 0;
    auto w = world->rnd->getInt(5, node->w - buffer);
    auto h = world->rnd->getInt(5, std::min({node->h - buffer, w}), 0.6*w);
    auto x = world->rnd->getInt(node->x - 1, node->x + node->w - w - 1);
    auto y = world->rnd->getInt(node->y - 1, node->y + node->h - h - 1);
    auto roomBuilt = buildRoom(x, y, w, h, false);
    if(roomBuilt) {
      Avoid::Rectangle roomRect(Avoid::Point(x, y), w+1, h+1);
      Avoid::ShapeRef *roomShape = new Avoid::ShapeRef(&router, roomRect);
      if (prevPos.x != -1) {
        edges.emplace_back(prevPos, Position(x, y));
      }
      prevPos.x = x;
      prevPos.y = y;
    }
  }
  return true;
}
