#ifndef _WORLD_HPP
#define _WORLD_HPP

struct Display;
struct Map;

#include "Display.hpp"
#include "Map.hpp"
#include <entityx/entityx.h>
#include <libtcod.hpp>
#include <memory>
#include <string>

#include "AISystem.hpp"
#include "AttackSystem.hpp"
#include "Display.hpp"
#include "DisplaySystem.hpp"
#include "HealthSystem.hpp"
#include "Map.hpp"
#include "MovementSystem.hpp"
#include "World.hpp"
#include "components.hpp"
#include "events.hpp"

struct World : std::enable_shared_from_this<World> {
  entityx::EntityX ECS;
  entityx::Entity player;
  int turnCount = 0;
  entityx::EventManager &events = ECS.events;
  entityx::EntityManager &entities = ECS.entities;
  std::shared_ptr<Map> currLevel;

  TCODRandom *rnd;

  World() {}

  void init(std::shared_ptr<Display> d);
  void generateMap();
  void setupPlayer();
  void startSystems(std::shared_ptr<Display> d);
  void updateDisplayOnly();
  void update();
  int entitiesAt(std::vector<entityx::Entity> &results, int x, int y);
};

#endif
