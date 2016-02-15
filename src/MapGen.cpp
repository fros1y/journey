//
// Created by Martin Galese on 2/13/16.
//

#include <libtcod.hpp>
#include "MapGen.hpp"
#include "utils.h"
#include <boost/range/irange.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/make_connected.hpp>

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

void MapGen::init() {
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
  typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;
  typedef boost::graph_traits<Graph>::vertex_iterator vertex_iterator;

  Graph map_graph;

  boost::random::mt19937 rng;
  boost::generate_random_graph(map_graph, 10, 10, rng, true, true);
  boost::make_connected(map_graph);

  std::pair<edge_iterator, edge_iterator> ei = boost::edges(map_graph);
  std::pair<vertex_iterator, vertex_iterator> vi = boost::vertices(map_graph);

  std::map<int, std::shared_ptr<Room> > rooms;

  bool playerRoom = true;
  for (vertex_iterator it = vi.first; it != vi.second; ++it) {
    int x, y;

    if(playerRoom) {
      x = world->currLevel->width/2;
      y = world->currLevel->height/2;
      playerRoom = false;
    } else {
      x = world->rnd->getInt(2, width - 2);
      y = world->rnd->getInt(2, height - 2);
    }

    auto w = world->rnd->getInt(3, 20);
    auto h = world->rnd->getInt(3, 20);
    auto room = std::make_shared<Room>(Position(x, y), w, h);
    rooms.insert(std::pair<int, std::shared_ptr<Room> >(*it, room));
    buildRoom(*room);
  }

  for (edge_iterator it = ei.first; it != ei.second; ++it) {
    buildCorridor(*rooms[boost::source(*it, map_graph)], *rooms[boost::target(*it, map_graph)]);
  }
}

bool MapGen::buildRoom(const Room &r) {
  auto leftMost = int(r.center.x - r.width / 2.0);
  auto rightMost = int(r.center.x + r.width / 2.0);
  auto topMost = int(r.center.y - r.height / 2.0);
  auto bottomMost = int(r.center.y + r.height / 2.0);
  return rectFill(leftMost, rightMost, topMost, bottomMost, Element::Floor);
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
    do {} while (step(x, r2.center.x, size) || step(y, r2.center.y, size));
  };

  repeatSteps(13);
  repeatSteps(7);
  repeatSteps(5);
  repeatSteps(3);
  repeatSteps(2);
  repeatSteps(1);

  return true;
}
