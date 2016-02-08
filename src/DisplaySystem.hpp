#ifndef _DISPLAYSYSTEM_HPP
#define _DISPLAYSYSTEM_HPP

#include <entityx/entityx.h>
#include <memory>
#include <string>
#include "World.hpp"
#include "events.hpp"

struct DisplaySystem : entityx::System<DisplaySystem>,
                       public entityx::Receiver<DisplaySystem> {
  std::shared_ptr<Display> display;
  std::shared_ptr<World> world;

  DisplaySystem(std::shared_ptr<Display> display, std::shared_ptr<World> world)
      : display(display), world(world) {}

  void configure(entityx::EventManager &event_manager) override;

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

  void receive(const Message &mesg);
};

#endif
