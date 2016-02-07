#ifndef _DJIKSTRAMAP_HPP
#define _DJIKSTRAMAP_HPP

#include <vector>

struct DjikstraMap {
  int width;
  int height;
  std::vector<int> cells;

  DjikstraMap(int width, int height)
      : width(width), height(height), cells(width * height, 9999) {}

  void calculate(int d_x, int d_y);
  void nextStepFrom(int x, int y, int &n_x, int &n_y);
  bool canReachFrom(int x, int y);
  void setProperties(int x, int y, bool traversable);

  int get(int x, int y) {
    if (x < 0 || y < 0 || x > width || y > height)
      return 9999;
    return cells[width * y + x];
  }

  void set(int x, int y, int v) { cells[width * y + x] = v; }
};
#endif
