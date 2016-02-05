#ifndef _EVENTS_HPP
#define _EVENTS_HPP

struct Movement {
  mutable entityx::Entity target;
  int d_x;
  int d_y;

  Movement(entityx::Entity target, int d_x, int d_y)
      : target(target), d_x(d_x), d_y(d_y) {}
};

struct Attack {
  mutable entityx::Entity source;
  mutable entityx::Entity target;
  Attack(entityx::Entity source, entityx::Entity target, int damage = 1)
      : source(source), target(target) {}
};

struct Damage {
  mutable entityx::Entity target;
  int amount;
  Damage(entityx::Entity target, int amount) : target(target), amount(amount) {}
};

struct Message {
  std::string text;
  Message(std::string text) : text(text) {}
};

#endif
