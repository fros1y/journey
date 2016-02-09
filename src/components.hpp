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
    TCODColor color;
    bool known;

    Render(Glyph glyph, TCODColor color = TCODColor::white) : glyph(glyph), color(color), known(false) { }
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
    bool stationary;
    bool unique;

    AI(std::string name, bool stationary = false, bool unique = false) : name(name), stationary(stationary),
                                                                         unique(unique) { }
};

struct Obstruction {
    bool obstructs;
    bool blocksView;

    Obstruction(bool obstructs = true, bool blocksView = true)
            : obstructs(obstructs), blocksView(blocksView) { }
};

#endif
