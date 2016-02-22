#ifndef _DISPLAY_HPP
#define _DISPLAY_HPP

struct World;

#include <libtcod.hpp>
#include <memory>
#include <string>
#include "World.hpp"
#include "components.hpp"
#include <deque>

struct Display {
  int width;
  int height;

  int camera_x;
  int camera_y;

  const int MESG_HEIGHT = 3;
  const int STATUS_HEIGHT = 1;

  std::shared_ptr<TCODConsole> levelView;
  std::shared_ptr<TCODConsole> statusBarView;
  std::shared_ptr<TCODConsole> messageBarView;

  std::deque<std::string> messages;

  Display(int width = 100, int height = 50, int camera_x = 0, int camera_y = 0)
      : width(width), height(height), camera_x(camera_x), camera_y(camera_y), messages() { }

  void init(void);
  void clear();

  void drawEntity(const int x, const int y, const Glyph g, const TCODColor &color = TCODColor::white);

  void statusBar(std::shared_ptr<World> world);

  void showMessages();
  void addMessage(const std::string &msg);

  void render();

};

#endif
