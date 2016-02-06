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
  void basicMotion(entityx::Entity e);

  bool failMoraleCheck();
  bool canMoveAway();
  void moveAway();
  bool hasRangedAttack();
  bool canMoveToward();
  bool decideToCharge();
  void moveToward();
  void rangedAttack();
  bool canAttack();
  bool decideToRetreat();
  void attack();
  void wait();
};

#endif
