#include "AISystem.hpp"

void AISystem::update(entityx::EntityManager &es, entityx::EventManager &events,
                      entityx::TimeDelta dt) {
  entityx::ComponentHandle<Position> player_pos =
      world->player.component<Position>();
  target_x = player_pos->x;
  target_y = player_pos->y;

  es.each<NPC>([this](entityx::Entity entity, NPC &npc) {
    world->currLevel->calculateMaps();
    basicMotion(entity);
  });
}

bool AISystem::failMoraleCheck() { return false; }

bool AISystem::canMoveAway() { return false; }

void AISystem::moveAway() {}

bool AISystem::hasRangedAttack() { return false; }

bool AISystem::canMoveToward(entityx::Entity e) {
  entityx::ComponentHandle<Position> AI_pos = e.component<Position>();
  return world->currLevel->canReachFrom(AI_pos->x, AI_pos->y);
}

bool AISystem::decideToCharge() { return false; }

void AISystem::moveToward(entityx::Entity e) {
  entityx::ComponentHandle<Position> AI_pos = e.component<Position>();
  // world->currLevel->djikstra_map->setPath(AI_pos->x, AI_pos->y);
  // world->currLevel->djikstra_map->reverse();
  int g_x, g_y;
  world->currLevel->nextStepFrom(AI_pos->x, AI_pos->y, g_x, g_y);
  world->events.emit<Movement>(e, g_x - AI_pos->x, g_y - AI_pos->y);
}

void AISystem::rangedAttack() {}

bool AISystem::canAttack() { return false; }

bool AISystem::decideToRetreat() { return false; }

void AISystem::attack() {}

void AISystem::wait() {}

void AISystem::basicMotion(entityx::Entity e) {
  world->currLevel->computeMovesTo(target_x, target_y);
  if (canMoveToward(e)) {
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

void AISystem::randomMotion(entityx::Entity e) {
  int d_x = world->rnd->getInt(-1, 1);
  int d_y = world->rnd->getInt(-1, 1);
  world->events.emit<Movement>(e, d_x, d_y);
}

void AISystem::zombieMotion(entityx::Entity e) {
  int d_x = 0;
  int d_y = 0;
  entityx::ComponentHandle<Position> entity_pos = e.component<Position>();
  entityx::ComponentHandle<Position> player_pos =
      world->player.component<Position>();

  if (entity_pos->x > player_pos->x) {
    d_x--;
  } else if (entity_pos->x < player_pos->x) {
    d_x++;
  }

  if (entity_pos->y > player_pos->y) {
    d_y--;
  } else if (entity_pos->y < player_pos->y) {
    d_y++;
  }

  world->events.emit<Movement>(e, d_x, d_y);
}
