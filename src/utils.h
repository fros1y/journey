//
// Created by Martin Galese on 2/8/16.
//
#include <algorithm>
#include <functional>

#ifndef JOURNEY_UTILS_H
#define JOURNEY_UTILS_H

int min(std::__1::vector<int> list);


template <typename T>
T clamp(T in, T low, T high)
{
  return std::min(std::max(in, low), high);
}

template<typename F>
void forXY(const int max_x, const int max_y, F function) {
  for(auto i = 0; i <= max_x; i++) {
    for(auto j = 0; j <= max_y; j++) {
      function(i, j);
    }
  }
}


#endif //JOURNEY_UTILS_H
