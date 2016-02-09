#ifndef _EVENTS_HPP
#define _EVENTS_HPP

#include <entityx/entityx.h>

struct Movement {
  mutable entityx::Entity target;
  int d_x;
  int d_y;

  Movement(const entityx::Entity &target, const int d_x, const int d_y)
      : target(target), d_x(d_x), d_y(d_y) {}
};

struct Attack {
  mutable entityx::Entity source;
  mutable entityx::Entity target;
  Attack(const entityx::Entity &source, const entityx::Entity &target, const int damage = 1)
      : source(source), target(target) {}
};

struct Damage {
  mutable entityx::Entity target;
  int amount;
  Damage(const entityx::Entity &target, const int amount) : target(target), amount(amount) {}
};

struct Message {
  std::string text;
  Message(const std::string &text) : text(text) {}
};

#endif
