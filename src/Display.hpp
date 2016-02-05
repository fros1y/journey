#ifndef _DISPLAY_HPP
#define _DISPLAY_HPP
#include "World.hpp"
#include <libtcod.hpp>

struct Display {
  int width;
  int height;

  TCODConsole *levelView;
  TCODConsole *statusBarView;

  Display(int width = 40, int height = 20) : width(width), height(height) {}
  void init(void) {
    TCODConsole::setCustomFont("bfont.png", TCOD_FONT_LAYOUT_ASCII_INROW, 16,
                               16);
    TCODConsole::initRoot(width, height, "Journey", false);

    levelView = new TCODConsole(width, height - 1);
    statusBarView = new TCODConsole(width, 1);
  }

  void clear() {
    levelView->clear();
    statusBarView->clear();
  }

  void drawEntity(int x, int y, Glyph g) { levelView->putChar(x, y, g); }

  void statusBar(std::shared_ptr<World> world) {
    entityx::ComponentHandle<Health> health = world->player.component<Health>();
    statusBarView->print(0, 0, "%i/%i\tTurn: %i", health->currHP, health->maxHP,
                         world->turnCount);
  }

  void render() {
    TCODConsole::blit(levelView, 0, 0, 0, 0, TCODConsole::root, 0, 0);
    TCODConsole::blit(statusBarView, 0, 0, 0, 0, TCODConsole::root, 0,
                      height - 1);
    TCODConsole::flush();
  }

  ~Display() {
    delete levelView;
    delete statusBarView;
  }
};

#endif
