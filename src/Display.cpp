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

void Display::drawEntity(const int x, const int y, const Glyph g, const TCODColor &color) {
    int sc_x = x - camera_x;
    int sc_y = y - camera_y;
    levelView->putChar(sc_x, sc_y, g);
    levelView->setCharForeground(sc_x, sc_y, color);
}

void Display::statusBar(std::shared_ptr<World> world) {
    entityx::ComponentHandle<Health> health = world->player.component<Health>();
    statusBarView->setDefaultBackground(TCODColor::lightestGrey);
    statusBarView->setDefaultForeground(TCODColor::black);

    statusBarView->print(0, 0, "%i/%i", health->currHP, health->maxHP);
    statusBarView->printEx(width - 1, 0, statusBarView->getBackgroundFlag(),
                           TCOD_RIGHT, "Turn: %i", world->turnCount);
}

void Display::render() {
    TCODConsole::blit(levelView, 0, 0, 0, 0, TCODConsole::root, 0, 0);
    TCODConsole::blit(statusBarView, 0, 0, 0, 0, TCODConsole::root, 0,
                      height - 1);
    TCODConsole::flush();
}
