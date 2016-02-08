#ifndef _MOVEMENTSYSTEM_HPP
#define _MOVEMENTSYSTEM_HPP

#include <entityx/entityx.h>
#include <memory>
#include <string>
#include "World.hpp"
#include "events.hpp"

bool obstructsTest(entityx::Entity e);
bool NPCTest(entityx::Entity e);

int npc_filter(std::vector<entityx::Entity> &results,
               std::vector<entityx::Entity> input);

struct MovementSystem : entityx::System<MovementSystem>,
                        public entityx::Receiver<MovementSystem> {
  std::shared_ptr<World> world;
  MovementSystem(std::shared_ptr<World> world) : world(world) {}

  void update(entityx::EntityManager &entities, entityx::EventManager &events,
              entityx::TimeDelta dt) override {}

  void configure(entityx::EventManager &event_manager) override;

  void receive(const Movement &move);
};

#endif
