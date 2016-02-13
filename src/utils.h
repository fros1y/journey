//
// Created by Martin Galese on 2/8/16.
//
#include <algorithm>

#ifndef JOURNEY_UTILS_H
#define JOURNEY_UTILS_H

int min(std::__1::vector<int> list);


template <typename T>
T clamp(T in, T low, T high)
{
  return std::min(std::max(in, low), high);
}

#endif //JOURNEY_UTILS_H
