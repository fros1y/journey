#ifndef _components_HPP
#define _components_HPP

#include <memory>
#include <string>
#include <libtcod.hpp>

typedef char Glyph;

struct LightSource {
  float brightness;
  TCODColor color;

  LightSource(const float brightness = 1, const TCODColor &color = TCODColor::lightYellow)
      : brightness(brightness), color(color) { }
};

struct Attackable {
  Attackable() { }
};

struct Render {
  Glyph glyph;
  TCODColor color;
  bool known;

  Render(const Glyph glyph, const TCODColor &color = TCODColor::white) : glyph(glyph), color(color), known(false) { }
};

struct Position {
  Position(const int x = 0, const int y = 0) : x(x), y(y) { }

  int x, y;
};

struct Health {
  Health(const int maxHP, const int currHP) : maxHP(maxHP), currHP(currHP) { }

  Health(const int HP) : Health(HP, HP) { }

  int maxHP;
  int currHP;
};

struct AI {
  std::string name;
  bool stationary;
  bool unique;

  AI(const std::string name, const bool stationary = false, const bool unique = false)
      : name(name), stationary(stationary),
        unique(unique) { }
};

struct Obstruction {
  bool obstructs;
  bool blocksView;

  Obstruction(const bool obstructs = true, const bool blocksView = true)
      : obstructs(obstructs), blocksView(blocksView) { }
};

#endif
