#ifndef LIGHTMAP_HPP
#define LIGHTMAP_HPP

#include <vector>

struct LightMap {
  int width;
  int height;
  std::vector<int> lightLevel;

  LightMap(int width, int height)
      : width(width), height(height), lightLevel(width * height, 0) {}

  bool illuminated(int x, int y);

  int get(int x, int y) {
    if (x < 0 || y < 0 || x > width || y > height) return -1;
    return lightLevel[width * y + x];
  }

  void set(int x, int y, int v) { lightLevel[width * y + x] = v; }
};

#endif  // LIGHTMAP_HPP
