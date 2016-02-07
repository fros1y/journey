#include "DjikstraMap.hpp"
#include <algorithm>

int min(std::vector<int> list) {
  return *(min_element(list.begin(), list.end()));
}

void DjikstraMap::calculate(int d_x, int d_y) {
  bool changeMade = true;
  set(d_x, d_y, 0);

  while (changeMade) {
    changeMade = false;
    for (auto i = 0; i < width; i++) {
      for (auto j = 0; j < height; j++) {
        int val = get(i, j);
        if (val == 9999)
          continue;
        std::vector<int> directions = {get(i + 1, j), get(i - 1, j),
                                       get(i, j + 1), get(i, j - 1)};
        int lowest_neighboor = min(directions);
        if (val - lowest_neighboor > 2) {
          changeMade = true;
          set(i, j, lowest_neighboor + 1);
        }
      }
    }
  }

  // for (auto i = 0; i < width; i++) {
  //   for (auto j = 0; j < height; j++) {
  //     if (get(i, j) == 9999)
  //       printf("[.]");
  //     else
  //       printf("[%i]", get(i, j));
  //   }
  //   printf("\n");
  // }
}

void DjikstraMap::nextStepFrom(int x, int y, int &n_x, int &n_y) {
  n_x = x;
  n_y = y;

  int up = get(x, y - 1);
  int down = get(x, y + 1);
  int left = get(x - 1, y);
  int right = get(x + 1, y);
  std::vector<int> directions = {up, down, left, right};

  int minimum = min(directions);
  if (minimum == up)
    n_y = y - 1;
  else if (minimum == down)
    n_y = y + 1;
  else if (minimum == left)
    n_x = x - 1;
  else if (minimum == right)
    n_x = x + 1;

  printf("[%i,%i]=>[%i, %i]\n", x, y, n_x, n_y);
}

bool DjikstraMap::canReachFrom(int x, int y) { return true; }

void DjikstraMap::setProperties(int x, int y, bool traversable) {
  if (traversable)
    set(x, y, 999);
  else
    set(x, y, 9999);
}
