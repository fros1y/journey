#include "Display.hpp"
#include <algorithm>
#include <limits>

void Display::init(void) {
  // TCODConsole::setCustomFont("bfont.png", TCOD_FONT_LAYOUT_ASCII_INROW, 16,
  // 16);
  TCODConsole::setCustomFont(
      "terminal8x14_gs_ro.png",
      TCOD_FONT_TYPE_GRAYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW, 16, 16);

  TCODConsole::initRoot(width, height, "Journey", false);

  levelView = std::make_shared<TCODConsole>(width, height - MESG_HEIGHT - STATUS_HEIGHT);
  statusBarView = std::make_shared<TCODConsole>(width, STATUS_HEIGHT);
  messageBarView = std::make_shared<TCODConsole>(width, MESG_HEIGHT);
}

void Display::clear() {
  levelView->clear();
  statusBarView->clear();
  messageBarView->clear();
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

void Display::addMessage(const std::string &msg) {
  messages.push_back(msg);
  if(messages.size() > (unsigned long)MESG_HEIGHT)
    messages.pop_front();
}

void Display::showMessages() {
  for(unsigned int i = 0; i < std::min(messages.size(), (unsigned long)MESG_HEIGHT); i++) {
    messageBarView->print(0, i, messages[i].c_str());
  }
}

void Display::render() {
  TCODConsole::blit(levelView.get(), 0, 0, 0, 0, TCODConsole::root, 0, 0);
  TCODConsole::blit(statusBarView.get(), 0, 0, 0, 0, TCODConsole::root, 0,
                    height - (MESG_HEIGHT + STATUS_HEIGHT));
  TCODConsole::blit(messageBarView.get(), 0, 0, 0, 0, TCODConsole::root, 0,
                    height - (MESG_HEIGHT));
  TCODConsole::flush();
}
