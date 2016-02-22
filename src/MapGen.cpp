//
// Created by Martin Galese on 2/16/16.
//

#include "MapGen.hpp"
#include <boost/range/irange.hpp>
#include "utils.h"

std::vector<Position> _BSP_libavoid_storage;
void _BSP_libavoid_callback(void *ptr) {
  Avoid::ConnRef *connRef = (Avoid::ConnRef *) ptr;
  const Avoid::PolyLine &route = connRef->route();
  for (auto i = 0; i < route.ps.size() - 1; ++i) {
    int x = int(route.ps[i].x);
    int y = int(route.ps[i].y);
    TCODLine::init(x, y, int(route.ps[i + 1].x), int(route.ps[i + 1].y));
    do {
      _BSP_libavoid_storage.emplace_back(x, y);
    } while (!TCODLine::step(&x, &y));
  }
}

void MapGen::generate() {
  _BSP_libavoid_storage.clear();
  auto BSP = std::__1::make_unique<TCODBsp>(BSPEdgeBuffer, BSPEdgeBuffer, width - BSPEdgeBuffer, height - BSPEdgeBuffer);
  BSP->splitRecursive(world->rnd, MaxDepth, BSPsizeH, BSPsizeV, BSPmaxHRatio, BSPmaxWRatio);
  BSP->traverseInOrder(this, NULL);
  for (auto edge : edges) {
    auto srcP = edge.first;
    auto destP = edge.second;
    Avoid::Point srcPt(srcP.x, srcP.y);
    Avoid::Point destPt(destP.x, destP.y);
    auto connRef = new Avoid::ConnRef(&router, srcPt, destPt);
    connRef->setCallback(&_BSP_libavoid_callback, connRef);
  }
  router.processTransaction();
  for (auto p : _BSP_libavoid_storage) {
    map[clamp(p.x, 1, width - 2)][clamp(p.y, 1, width - 2)] = Element::Floor;
  }
  world->currLevel->playerStart = findFree();
}

bool MapGen::visitNode(TCODBsp *node, void *userData) {
  if (node->isLeaf()) {
    auto w = world->rnd->getInt(minRegionW, node->w - RegionBuffer);
    auto h = world->rnd->getInt(minRegionH, std::__1::min({node->h - RegionBuffer, w}), 0.6 * w);
    auto x = world->rnd->getInt(node->x - 1, node->x + node->w - w - 1);
    auto y = world->rnd->getInt(node->y - 1, node->y + node->h - h - 1);
    auto regionBuilt = buildRegion(x, y, w, h, false);
    if (regionBuilt) {
      Avoid::Rectangle regionRect(Avoid::Point(x, y), w + 1, h + 1);
      Avoid::ShapeRef *regionShape = new Avoid::ShapeRef(&router, regionRect);
      if (prevPos.x != -1) {
        edges.emplace_back(prevPos, Position(x, y));
      }
      prevPos.x = x;
      prevPos.y = y;
    }
  }
  return true;
}

Position MapGen::findFree() {
  int x, y;
  do {
    x = world->rnd->getInt(1, width - 1);
    y = world->rnd->getInt(1, height - 1);
  } while (map[x][y] != Element::Floor);
  return
      Position(x, y
      );
}

bool MapGen::ellipseFill(int leftMost,
                         int rightMost,
                         int topMost,
                         int bottomMost,
                         Element fill, bool squash) {

  leftMost = clamp(leftMost, 1, width - 2);
  rightMost = clamp(rightMost, 1, width - 2);
  topMost = clamp(topMost, 1, height - 2);
  bottomMost = clamp(bottomMost, 1, height - 2);

  int xaxis = int((rightMost - leftMost) / 2.0);
  int yaxis = int((bottomMost - topMost) / 2.0);

  auto xcenter = (rightMost + leftMost) / 2.0;
  auto ycenter = (topMost + bottomMost) / 2.0;

  if (!squash) {
    for (auto i : boost::irange(leftMost, rightMost)) {
      for (auto j : boost::irange(topMost, bottomMost)) {
        if (map[i][j] != Element::Rock) {
          return false;
        }
      }
    }
  }

  for (auto i : boost::irange(leftMost, rightMost + 1)) {
    for (auto j : boost::irange(topMost, bottomMost + 1)) {

      auto x = i - xcenter;
      auto y = j - ycenter;

      auto test = (x * x) / (xaxis * xaxis) + (y * y) / (yaxis * yaxis);
      if (test <= .9)
        map[i][j] = Element::Floor;
    }
  }
  return true;
}

bool MapGen::rectFill(const int leftMostIn,
                      const int rightMostIn,
                      const int topMostIn,
                      const int bottomMostIn,
                      const Element fill, const bool squash) {

  auto leftMost = clamp(leftMostIn, 1, width - 2);
  auto rightMost = clamp(rightMostIn, 1, width - 2);
  auto topMost = clamp(topMostIn, 1, height - 2);
  auto bottomMost = clamp(bottomMostIn, 1, height - 2);

  if (!squash) {
    for (auto i : boost::irange(leftMost, rightMost)) {
      for (auto j : boost::irange(topMost, bottomMost)) {
        if (map[i][j] != Element::Rock) {
          return false;
        }
      }
    }
  }

  for (auto i : boost::irange(leftMost, rightMost)) {
    for (auto j : boost::irange(topMost, bottomMost)) {
      map[i][j] = fill;
    }
  }
  return true;
}

void MapGen::floodFill(const int x, const int y, const Element fill) {
  if (map[x][y] != Element::Empty)
    return;
  else
    map[x][y] = fill;
  if (y > 0) floodFill(x, y - 1, fill);
  if (x > 0) floodFill(x - 1, y, fill);
  if (x > 0 && y > 0) floodFill(x - 1, y - 1, fill);
  if (x < width - 1) floodFill(x + 1, y, fill);
  if (y < height - 1) floodFill(x, y + 1, fill);
  if (x < width - 1 && y < height - 1) floodFill(x + 1, y + 1, fill);
  if (x > 0 && y < height - 1) floodFill(x - 1, y + 1, fill);
  if (x < width - 1 && y > 0) floodFill(x + 1, y - 1, fill);
}

bool MapGen::buildRegion(const int x, const int y, int width, int height, bool squash) {

  auto leftMost = int(x - width / 2.0);
  auto rightMost = int(x + width / 2.0);
  auto topMost = int(y - height / 2.0);
  auto bottomMost = int(y + height / 2.0);

  auto rnd = world->rnd->getInt(0, 100);
  switch (rnd) {
    case 0 ... 10:
      return ellipseFill(leftMost, rightMost, topMost, bottomMost, Element::Floor, squash);
    default:
      return rectFill(leftMost, rightMost, topMost, bottomMost, Element::Floor, squash);
  }
}