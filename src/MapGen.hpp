//
// Created by Martin Galese on 2/16/16.
//

#ifndef JOURNEY_MAPGEN_HPP
#define JOURNEY_MAPGEN_HPP

#include "components.hpp"
#include "world.hpp"
#include <vector>

enum class Element { Empty, Floor, Rock, Door };
using Edge = std::pair<Position, Position>;

struct MapGen: public ITCODBspCallback {
  std::shared_ptr<World> world;
  int width, height;
  std::vector<std::vector<Element>> map;
  Position prevPos{-1, -1};
  std::vector<Edge> edges;

  const int MaxDepth = 8;
  const int BSPsizeH = 10;
  const int BSPsizeV = 10;
  const float BSPmaxHRatio = 1.5f;
  const float BSPmaxWRatio = 1.5f;
  const int BSPEdgeBuffer = 10;
  const int minRegionW = 10;
  const int minRegionH = 10;
  const int RegionBuffer = 5;

  MapGen(std::shared_ptr<World> world, const int width, const int height)
      : world(world), width(width), height(height), edges() {

    map.resize(width, std::vector<Element>(height, Element::Rock));
  }

  void generate();
  bool visitNode(TCODBsp *node, void *userData) override;
  bool buildRegion(const int x, const int y, int width, int height, bool squash = false);

  Position findFree();

  template<typename F>
  void forAll(F function) {
    for (auto i = 0; i < width; i++) {
      for (auto j = 0; j < height; j++) {
        function(map[i][j], i, j);
      }
    }
  }

  bool ellipseFill(int leftMost,
                   int rightMost,
                   int topMost,
                   int bottomMost,
                   Element fill, bool squash = false);

  bool rectFill(const int leftMostIn,
                const int rightMostIn,
                const int topMostIn,
                const int bottomMostIn,
                const Element fill,
                const bool squash = false);

  void floodFill(const int x, const int y, const Element fill);




};


#endif //JOURNEY_MAPGEN_HPP
