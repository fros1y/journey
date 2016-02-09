#ifndef _DJIKSTRAMAP_HPP
#define _DJIKSTRAMAP_HPP

#include <vector>

struct DjikstraMap {
  int width;
  int height;
  std::vector<int> cells;

  DjikstraMap(const int width, const int height)
      : width(width), height(height), cells(width * height, 9999) {}

  void calculate(const int d_x, const int d_y);
  void nextStepFrom(const int x, const int y, int &n_x, int &n_y);
  bool canReachFrom(const int x, const int y);
  void setProperties(const int x, const int y, const bool traversable);

  int get(const int x, const int y) {
    if (x < 0 || y < 0 || x > width || y > height) return 9999;
    return cells[width * y + x];
  }

  void set(const int x, const int y, const int v) { cells[width * y + x] = v; }
};
#endif
