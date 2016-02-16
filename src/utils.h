//
// Created by Martin Galese on 2/8/16.
//
#include <algorithm>
#include <functional>
#include <assert.h>

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

template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

template <typename I, typename O> O maprange(I v, I in_min, I in_max, O out_min, O out_max)
{
  assert(v >= in_min);
  assert(v <= in_max);
  return (v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif //JOURNEY_UTILS_H
