#ifndef LIGHTMAP_HPP
#define LIGHTMAP_HPP

#include <vector>
#include <libtcod.h>
#include <color.hpp>

struct LightMap {
  int width;
  int height;
  std::vector<TCODColor> lightColor;

  LightMap(int width, int height)
      : width(width), height(height), lightColor(width * height, TCODColor::black) { }

  bool illuminated(int x, int y);

  TCODColor getColor(int x, int y) {
    if (x < 0 || y < 0 || x > width || y > height) return TCODColor::black;
    return lightColor[width * y + x];
  }

  void setColor(int x, int y, TCODColor v) { lightColor[width * y + x] = v; }
  void addColor(int x, int y, TCODColor c, float brightness) {
    lightColor[width * y + x] = lightColor[width * y + x] + c * brightness;
  }
};

#endif  // LIGHTMAP_HPP
