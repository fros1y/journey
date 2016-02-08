#include "LightMap.hpp"
#include <algorithm>


bool LightMap::illuminated(int x, int y) {
    return get(x, y) > 0;
}
