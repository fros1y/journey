#ifndef _components_HPP
#define _components_HPP

#include <memory>
#include <string>

typedef char Glyph;

struct Attackable {
  Attackable() {}
};

struct Render {
  Render(Glyph glyph) : glyph(glyph) {}
  Glyph glyph;
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

struct NPC {
  std::string name;
  bool unique;
  NPC(std::string name, bool unique = false) : name(name), unique(unique) {}
};

struct Obstruction {
  bool obstructs;
  bool obstructsAbove;
  bool obstructsBelow;

  Obstruction(bool obstructs = true, bool obstructsAbove = true,
              bool obstructsBelow = true)
      : obstructs(obstructs), obstructsAbove(obstructsAbove),
        obstructsBelow(obstructsBelow) {}
};

#endif
