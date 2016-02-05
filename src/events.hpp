#ifndef _EVENTS_HPP
#define _EVENTS_HPP

struct Movement {
  mutable entityx::Entity target;
  int d_x;
  int d_y;

  Movement(entityx::Entity target, int d_x, int d_y)
      : target(target), d_x(d_x), d_y(d_y) {}
};
#endif
