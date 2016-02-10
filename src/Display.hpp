#ifndef _DISPLAY_HPP
#define _DISPLAY_HPP

struct World;

#include <libtcod.hpp>
#include <memory>
#include <string>
#include "World.hpp"
#include "components.hpp"

struct Display {
  int width;
  int height;

  int camera_x;
  int camera_y;

  std::shared_ptr<TCODConsole> levelView;
  std::shared_ptr<TCODConsole> statusBarView;

  Display(int width = 80, int height = 40, int camera_x = 0, int camera_y = 0)
      : width(width), height(height), camera_x(camera_x), camera_y(camera_y) { }

  void init(void);
  void clear();

  void drawEntity(const int x, const int y, const Glyph g, const TCODColor &color = TCODColor::white);

  void statusBar(std::shared_ptr<World> world);

  void render();

};

#endif
