#ifndef _components_HPP
#define _components_HPP

typedef char Glyph;

struct Render {
  Render(Glyph glyph) : glyph(glyph) {}
  Glyph glyph;
};

struct Position {
  Position(int x = 0, int y = 0) : x(x), y(y) {}
  int x, y;
};

struct Health {
  Health(int maxHP = 100, int currHP = 100) : maxHP(maxHP), currHP(currHP) {}
  int maxHP;
  int currHP;
};

#endif
