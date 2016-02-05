#include <chrono>
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
  while (!TCODConsole::isWindowClosed()) {

    TCOD_key_t key = TCODConsole::checkForKeypress();
    switch (key.vk) {
    case TCODK_UP:
      d_y = -1;
      break;
    case TCODK_DOWN:
      d_y = 1;
      break;
    case TCODK_LEFT:
      d_x = -1;
      break;
    case TCODK_RIGHT:
      d_x = 1;
      break;
    default:
      world->updateDisplayOnly();
      continue;
      break;
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
