#include <chrono>
#include <memory>
#include <thread>

#include <entityx/entityx.h>
#include <libtcod.hpp>

#include "AttackSystem.hpp"
#include "Display.hpp"
#include "DisplaySystem.hpp"
#include "HealthSystem.hpp"
#include "Map.hpp"
#include "MovementSystem.hpp"
#include "World.hpp"
#include "components.hpp"
#include "events.hpp"

void gameLoop(std::shared_ptr<World> world, std::shared_ptr<Display> display) {
  int d_x = 0;
  int d_y = 0;

  TCODConsole::setKeyboardRepeat(200, 200);
  TCODSystem::setFps(30);

  bool quit = false;

  while (!TCODConsole::isWindowClosed() && !quit) {
    TCOD_key_t key = TCODConsole::checkForKeypress();
    switch (key.c) {
      case 'k':
        d_y = -1;
        break;
      case 'j':
        d_y = 1;
        break;
      case 'h':
        d_x = -1;
        break;
      case 'l':
        d_x = 1;
        break;
      case 'y':
        d_y = -1;
        d_x = -1;
        break;
      case 'u':
        d_y = -1;
        d_x = 1;
        break;
      case 'n':
        d_y = 1;
        d_x = -1;
        break;
      case 'm':
        d_y = 1;
        d_x = 1;
        break;
      case 'q':
        quit=true;
        break;
      default:
        world->updateDisplayOnly();
        continue;
    }

    if ((d_x != 0) or (d_y != 0)) {
      world->events.emit<Movement>(world->player, d_x, d_y);
      d_x = 0;
      d_y = 0;
    }
    world->update();
  }
}

int main(void) {
  auto display = std::make_shared<Display>();
  display->init();
  auto world = std::make_shared<World>();
  world->init(display);
  gameLoop(world, display);
  return 0;
}
