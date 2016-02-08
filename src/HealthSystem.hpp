#ifndef _HEALTHSYSTEM_HPP
#define _HEALTHSYSTEM_HPP

#include <entityx/entityx.h>
#include <memory>
#include <string>
#include "World.hpp"
#include "events.hpp"

struct HealthSystem : entityx::System<HealthSystem>,
                      public entityx::Receiver<HealthSystem> {
  std::shared_ptr<World> world;
  HealthSystem(std::shared_ptr<World> world) : world(world) {}
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {}

  void configure(entityx::EventManager &event_manager) override;

  void receive(const Damage &damage);
};

#endif
