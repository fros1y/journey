//
// Created by Martin Galese on 2/13/16.
//

#include "MapGen.hpp"
#include "utils.h"
#include <boost/range/irange.hpp>

bool MapGen::rectFill(int leftMost,
                      int rightMost,
                      int topMost,
                      int bottomMost,
                      Element fill) {

  leftMost = clamp(leftMost, 1, width - 2);
  rightMost = clamp(rightMost, 1, width - 2);
  topMost = clamp(topMost, 1, height - 2);
  bottomMost = clamp(bottomMost, 1, height - 2);

  for (auto i : boost::irange(leftMost, rightMost + 1)) {
    for (auto j : boost::irange(topMost, bottomMost + 1)) {
      if (map[i][j] != Element::Rock) {
        return false;
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

bool MapGen::buildCorridor(const int x, const int y, const Direction grow) {
  auto length = world->rnd->getInt(4, width / 5);

  int leftMost, rightMost, topMost, bottomMost;

  switch (grow) {
    case Direction::Down:
      leftMost = x;
      rightMost = x;
      topMost = y;
      bottomMost = clamp(y + length, 1, height - 2);
      if (map[x][bottomMost + 1] != Element::Floor)
        return false;
      break;
    case Direction::Up:
      leftMost = x;
      rightMost = x;
      topMost = clamp(y - length, 1, height - 2);
      bottomMost = y;
      if (map[x][topMost - 1] != Element::Floor)
        return false;
      break;
    case Direction::Left:
      leftMost = clamp(x - length, 1, width - 2);
      rightMost = x;
      topMost = y;
      bottomMost = y;
      if (map[leftMost - 1][y] != Element::Floor)
        return false;
      break;
    case Direction::Right:
      leftMost = x;
      rightMost = clamp(x + length, 1, width - 2);
      topMost = y;
      bottomMost = y;
      if (map[rightMost + 1][y] != Element::Floor)
        return false;
      break;
    case Direction::None:
      return false;
      break;
  }

  return rectFill(leftMost, rightMost, topMost, bottomMost, Element::Floor);
}

bool MapGen::buildRoom(const int x, const int y, const Direction grow) {

  auto w = world->rnd->getInt(4, width / 4);
  auto h = world->rnd->getInt(4, height / 2);
  int leftMost, rightMost, topMost, bottomMost;

  switch (grow) {
    case Direction::Down:
      leftMost = int(x - w / 2.0);
      rightMost = int(x + w / 2.0);
      topMost = y;
      bottomMost = y + h;
      break;
    case Direction::Up:
      leftMost = int(x - w / 2.0);
      rightMost = int(x + w / 2.0);
      topMost = y - h;
      bottomMost = y;
      break;
    case Direction::Left:
      leftMost = x - w;
      rightMost = x;
      topMost = int(y - h / 2.0);
      bottomMost = int(y + h / 2.0);
      break;
    case Direction::Right:
      leftMost = x;
      rightMost = x + w;
      topMost = int(y - h / 2.0);
      bottomMost = int(y + h / 2.0);
      break;
    case Direction::None:
      leftMost = int(x - w / 2.0);
      rightMost = int(x + w / 2.0);
      topMost = int(y - h / 2.0);
      bottomMost = int(y + h / 2.0);
      break;
  }

  return rectFill(leftMost, rightMost, topMost, bottomMost, Element::Floor);
}


Direction MapGen::spaceAvailable(const int x, const int y) {
  if (x < 1 || y < 1 || x >= width - 1 || y >= width - 1)
    return Direction::None;
  if (map[x][y] != Element::Rock)
    return Direction::None;

  auto testLeft = map[x - 1][y] == Element::Rock ? 1 : 0;
  auto testRight = map[x + 1][y] == Element::Rock ? 1 : 0;
  auto testDown = map[x][y + 1] == Element::Rock ? 1 : 0;
  auto testUp = map[x][y - 1] == Element::Rock ? 1 : 0;
  auto count = testUp + testDown + testRight + testLeft;

  if (count != 3)
    return Direction::None;

  if (count == 3 && !testUp) {
    return Direction::Down;
  }
  if (count == 3 && !testDown) {
    return Direction::Up;
  }
  if (count == 3 && !testLeft) {
    return Direction::Right;
  }
  if (count == 3 && !testRight) {
    return Direction::Left;
  }
  return Direction::None;
}

Direction MapGen::findSpace(int &x, int &y, const int maxTries) {
  auto tries = 0;
  Direction growDirection;

  do {
    tries++;
    if (tries > maxTries) {
      return Direction::None;
    }
    x = world->rnd->getInt(1, width - 2);
    y = world->rnd->getInt(1, height - 2);
    growDirection = spaceAvailable(x, y);
  } while (growDirection == Direction::None);
  return growDirection;
}

void MapGen::init() {
  auto roomCount = (width*height)/500;
  auto x = 20;
  auto y = 10;
  auto tries = 0;
  Direction growDirection;

  buildRoom(width / 2, height / 2, Direction::None);
  for (auto i : boost::irange(0, roomCount)) {
    retry:
    if (tries > 10000)
      return;
    tries++;
    growDirection = findSpace(x, y, 10000);
    auto random = world->rnd->getInt(0, 100);
    auto result = true;
    if (random < 99) {
      result = buildCorridor(x, y, growDirection);
    } else {
      result = buildRoom(x, y, growDirection);
    }
    if (!result)
      goto retry;
  }
}
