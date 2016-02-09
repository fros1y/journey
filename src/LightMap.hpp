#ifndef LIGHTMAP_HPP
#define LIGHTMAP_HPP

#include <vector>
#include <libtcod.h>
#include <color.hpp>

struct LightMap {
    int width;
    int height;
    std::vector<int> lightLevel;
    std::vector<TCODColor> lightColor;

    LightMap(int width, int height)
            : width(width), height(height), lightLevel(width * height, 0), lightColor(width * height, TCODColor::black) { }

    bool illuminated(int x, int y);

    int getLight(int x, int y) {
        if (x < 0 || y < 0 || x > width || y > height) return -1;
        return lightLevel[width * y + x];
    }

    void setLight(int x, int y, int v) { lightLevel[width * y + x] = v; }


    TCODColor getColor(int x, int y) {
        if (x < 0 || y < 0 || x > width || y > height) return TCODColor::black;
        return lightColor[width * y + x];
    }

    void setColor(int x, int y, TCODColor v) { lightColor[width * y + x] = v; }
};

#endif  // LIGHTMAP_HPP
