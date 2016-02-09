#include "LightMap.hpp"
#include <algorithm>

bool LightMap::illuminated(int x, int y) { return getLight(x, y) > 0; }
