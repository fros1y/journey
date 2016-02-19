//
// Created by Martin Galese on 2/16/16.
//

#include "MapGen.hpp"
#include <boost/range/irange.hpp>
#include "utils.h"

bool MapGen::ellipseFill(int leftMost,
                         int rightMost,
                         int topMost,
                         int bottomMost,
                         Element fill, bool squash) {

  leftMost = clamp(leftMost, 1, width - 2);
  rightMost = clamp(rightMost, 1, width - 2);
  topMost = clamp(topMost, 1, height - 2);
  bottomMost = clamp(bottomMost, 1, height - 2);

  int xaxis = (rightMost - leftMost) / 2.0;
  int yaxis = (bottomMost - topMost) / 2.0;

  auto xcenter = (rightMost + leftMost) / 2.0;
  auto ycenter = (topMost + bottomMost) / 2.0;

  for (auto i : boost::irange(leftMost-1, rightMost + 2)) {
    for (auto j : boost::irange(topMost-1, bottomMost + 2)) {

      auto x = i - xcenter;
      auto y = j - ycenter;

      auto test = (x * x) / (xaxis * xaxis) + (y * y) / (yaxis * yaxis);
      if (test <= 0.9 && map[i][j] == Element::Floor && !squash)
        return false;
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

bool MapGen::rectFill(const Position &center, const int width, const int height, const Element fill, const bool squash) {
  auto leftMost = int(center.x - width / 2.0);
  auto rightMost = int(center.x + width / 2.0);
  auto topMost = int(center.y - height / 2.0);
  auto bottomMost = int(center.y + height / 2.0);
  return rectFill(leftMost, rightMost, topMost, bottomMost, fill, squash);
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
    for (auto i : boost::irange(leftMost-1, rightMost + 2)) {
      for (auto j : boost::irange(topMost-1, bottomMost + 2)) {
        if (map[i][j] != Element::Rock) {
          return false;
        }
      }
    }
  }

  for (auto i : boost::irange(leftMost, rightMost + 1)) {
    for (auto j : boost::irange(topMost, bottomMost + 1)) {
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


bool MapGen::buildRoom(const Room &r, bool squash) {
  auto leftMost = int(r.center.x - r.width / 2.0);
  auto rightMost = int(r.center.x + r.width / 2.0);
  auto topMost = int(r.center.y - r.height / 2.0);
  auto bottomMost = int(r.center.y + r.height / 2.0);

  auto rnd = world->rnd->getInt(0, 100);
  switch (rnd) {
    case 0 ... 25:
      return ellipseFill(leftMost, rightMost, topMost, bottomMost, Element::Floor, squash);
    default:
      return rectFill(leftMost, rightMost, topMost, bottomMost, Element::Floor, squash);
  }
}

bool MapGen::buildCorridor(const Room &r1, const Room &r2) {
  int x = r1.center.x;
  int y = r1.center.y;

  auto step = [&](int &var, const int &testVar, const int size) {
    if (abs(var - testVar) >= size) {
      for (auto i : boost::irange(0, size)) {
        var += sgn(testVar - var);
        map[x][y] = Element::Floor;
      }
      return true;
    }
    return false;
  };

  auto repeatSteps = [&](const int size) {
    bool xtest, ytest;
    do {
      xtest = step(x, r2.center.x, size);
      ytest = step(y, r2.center.y, size);
    } while (xtest || ytest);
  };


  repeatSteps(7);
  repeatSteps(5);
  repeatSteps(2);
  repeatSteps(13);
  repeatSteps(3);
  repeatSteps(1);

  return true;
}