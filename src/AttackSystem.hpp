#ifndef _ATTACKSYSTEM_HPP
#define _ATTACKSYSTEM_HPP

#include "World.hpp"
#include "events.hpp"
#include <entityx/entityx.h>
#include <memory>
#include <string>

struct AttackSystem : entityx::System<AttackSystem>,
                      public entityx::Receiver<AttackSystem> {
  std::shared_ptr<World> world;

  AttackSystem(std::shared_ptr<World> world) : world(world) {}

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {}

  void configure(entityx::EventManager &event_manager) override;

  void receive(const Attack &attack);
};

#endif
