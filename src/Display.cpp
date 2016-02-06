#include "Display.hpp"

void Display::init(void) {
  // TCODConsole::setCustomFont("bfont.png", TCOD_FONT_LAYOUT_ASCII_INROW, 16,
  // 16);
  TCODConsole::setCustomFont(
      "terminal8x14_gs_ro.png",
      TCOD_FONT_TYPE_GRAYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW, 16, 16);

  TCODConsole::initRoot(width, height, "Journey", false);

  levelView = new TCODConsole(width, height - 1);
  statusBarView = new TCODConsole(width, 1);
}

void Display::clear() {
  levelView->clear();
  statusBarView->clear();
}

void Display::drawEntity(int x, int y, Glyph g) {
  int sc_x = x - camera_x;
  int sc_y = y - camera_y;
  levelView->putChar(sc_x, sc_y, g);
}

void Display::statusBar(std::shared_ptr<World> world) {
  entityx::ComponentHandle<Health> health = world->player.component<Health>();
  statusBarView->print(0, 0, "%i/%i\tTurn: %i", health->currHP, health->maxHP,
                       world->turnCount);
}

void Display::render() {
  TCODConsole::blit(levelView, 0, 0, 0, 0, TCODConsole::root, 0, 0);
  TCODConsole::blit(statusBarView, 0, 0, 0, 0, TCODConsole::root, 0,
                    height - 1);
  TCODConsole::flush();
}
