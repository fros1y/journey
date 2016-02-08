#ifndef _components_HPP
#define _components_HPP

#include <memory>
#include <string>

typedef char Glyph;

struct LightSource {
  int brightness;
  LightSource(int brightness = 3) : brightness(brightness) {}
};

struct Attackable {
  Attackable() {}
};

struct Render {
  Glyph glyph;
  bool known;
  Render(Glyph glyph) : glyph(glyph), known(false) {}
};

struct Position {
  Position(int x = 0, int y = 0) : x(x), y(y) {}
  int x, y;
};

struct Health {
  Health(int maxHP, int currHP) : maxHP(maxHP), currHP(currHP) {}
  Health(int HP) : Health(HP, HP) {}
  int maxHP;
  int currHP;
};

struct AI {
  std::string name;
  bool unique;
  AI(std::string name, bool unique = false) : name(name), unique(unique) {}
};

struct Obstruction {
  bool obstructs;
  bool blocksView;

  Obstruction(bool obstructs = true, bool blocksView = true)
      : obstructs(obstructs), blocksView(blocksView) {}
};

#endif
