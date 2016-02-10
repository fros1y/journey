#include "LightMap.hpp"
#include <algorithm>

bool LightMap::illuminated(int x, int y) {
  return getColor(x,y).getValue() >= 0.3;
}
