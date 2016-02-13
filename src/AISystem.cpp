#include "AISystem.hpp"

void AISystem::update(entityx::EntityManager &es, entityx::EventManager &events,
                      entityx::TimeDelta dt) {
  entityx::ComponentHandle<Position> player_pos =
      world->player.component<Position>();
  target_x = player_pos->x;
  target_y = player_pos->y;
  auto player_speed = world->player.component<Speed>();

  es.each<AI,Speed>([this, player_speed](entityx::Entity entity, AI &ai, Speed &speed) {
    while (speed.ap > 1.0) {
      switch (ai.strategy) {
        case AIType::Basic:
          basicAI(entity);
          break;
        case AIType::Random:
          randomAI(entity);
          break;
        case AIType::Stationary:
          stationaryAI(entity);
          break;
      }
      speed.ap -= 1;
    }
    speed.ap += speed.speed / player_speed->speed;
  });
}

bool AISystem::failMoraleCheck() { return false; }

bool AISystem::canMoveAway() { return false; }

void AISystem::moveAway() { }

bool AISystem::hasRangedAttack() { return false; }

bool AISystem::canMoveToward(entityx::Entity &e) {
  entityx::ComponentHandle<Position> AI_pos = e.component<Position>();
  return world->currLevel->canReachFrom(AI_pos->x, AI_pos->y);
}

bool AISystem::decideToCharge() { return false; }

void AISystem::moveToward(entityx::Entity &e) {
  entityx::ComponentHandle<Position> AI_pos = e.component<Position>();
  int g_x, g_y;
  world->currLevel->nextStepFrom(AI_pos->x, AI_pos->y, g_x, g_y);
  world->events.emit<Movement>(e, g_x - AI_pos->x, g_y - AI_pos->y);
}

void AISystem::rangedAttack() { }

bool AISystem::canAttack() { return false; }

bool AISystem::decideToRetreat() { return false; }

void AISystem::attack() { }

void AISystem::wait() { }

bool AISystem::canSee(entityx::Entity &e) {
  entityx::ComponentHandle<Position> entity_pos = e.component<Position>();
  world->currLevel->computeFoVFrom(entity_pos->x, entity_pos->y, 5);
  return world->currLevel->isInFoV(target_x, target_y);
}

void AISystem::stationaryAI(entityx::Entity &e) {
  if (canSee(e) && canAttack()) {
    attack();
  }
}

void AISystem::basicAI(entityx::Entity &e) {
  world->currLevel->calculateMaps();
  world->currLevel->computeMovesTo(target_x, target_y);

  if (canSee(e) && canMoveToward(e)) {
    moveToward(e);
  }
  // if (failMoraleCheck() && canMoveAway()) {
  //   moveAway();
  // } else if (hasRangedAttack() && canMoveToward(e)) {
  //   if (decideToCharge()) {
  //     moveToward(e);
  //   } else {
  //     rangedAttack();
  //   }
  // } else if (canAttack() && hasRangedAttack() && canMoveAway()) {
  //   if (decideToRetreat()) {
  //     moveAway();
  //   } else {
  //     attack();
  //   }
  // } else if (canAttack()) {
  //   attack();
  // } else if (canMoveToward(e)) {
  //   moveToward(e);
  // } else {
  //   wait();
  // }
}

void AISystem::randomAI(entityx::Entity &e) {
  int d_x = world->rnd->getInt(-1, 1);
  int d_y = world->rnd->getInt(-1, 1);
  world->events.emit<Movement>(e, d_x, d_y);
}

