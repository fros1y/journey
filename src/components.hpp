#ifndef _components_HPP
#define _components_HPP

#include <memory>
#include <string>
#include <libtcod.hpp>

typedef char Glyph;

struct LightSource {
    int brightness;
    TCODColor color;

    LightSource(int brightness = 3, TCODColor color = TCODColor::lightYellow) : brightness(brightness), color(color) { }
};

struct Attackable {
    Attackable() { }
};

struct Render {
    Glyph glyph;
    bool known;
    TCODColor color;

    Render(Glyph glyph) : glyph(glyph), known(false), color(TCODColor::white) { }
};

struct Position {
    Position(int x = 0, int y = 0) : x(x), y(y) { }

    int x, y;
};

struct Health {
    Health(int maxHP, int currHP) : maxHP(maxHP), currHP(currHP) { }

    Health(int HP) : Health(HP, HP) { }

    int maxHP;
    int currHP;
};

struct AI {
    std::string name;
    bool unique;

    AI(std::string name, bool unique = false) : name(name), unique(unique) { }
};

struct Obstruction {
    bool obstructs;
    bool blocksView;

    Obstruction(bool obstructs = true, bool blocksView = true)
            : obstructs(obstructs), blocksView(blocksView) { }
};

#endif
