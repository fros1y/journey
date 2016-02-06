#ifndef _AISYSTEM_HPP
#define _AISYSTEM_HPP

#include "World.hpp"
#include "components.hpp"
#include "events.hpp"
#include <entityx/entityx.h>
#include <libtcod.hpp>
#include <memory>
#include <string>

struct AISystem : entityx::System<AISystem> {
  std::shared_ptr<World> world;

  AISystem(std::shared_ptr<World> world) : world(world) {}

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

  void randomMotion(entityx::Entity e);
  void zombieMotion(entityx::Entity e);
};

#endif
