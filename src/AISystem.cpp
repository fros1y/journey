#include "AISystem.hpp"

void AISystem::update(entityx::EntityManager &es, entityx::EventManager &events,
                      entityx::TimeDelta dt) {
  es.each<NPC>(
      [this](entityx::Entity entity, NPC &npc) { zombieMotion(entity); });
}

bool AISystem::failMoraleCheck() { return false; }

bool AISystem::canMoveAway() { return false; }

void AISystem::moveAway() {}

bool AISystem::hasRangedAttack() { return false; }

bool AISystem::canMoveToward() { return false; }

bool AISystem::decideToCharge() { return false; }

void AISystem::moveToward() {}

void AISystem::rangedAttack() {}

bool AISystem::canAttack() { return false; }

bool AISystem::decideToRetreat() { return false; }

void AISystem::attack() {}

void AISystem::wait() {}

void AISystem::basicMotion(entityx::Entity e) {
  if (failMoraleCheck() && canMoveAway()) {
    moveAway();
  } else if (hasRangedAttack() && canMoveToward()) {
    if (decideToCharge()) {
      moveToward();
    } else {
      rangedAttack();
    }
  } else if (canAttack() && hasRangedAttack() && canMoveAway()) {
    if (decideToRetreat()) {
      moveAway();
    } else {
      attack();
    }
  } else if (canAttack()) {
    attack();
  } else if (canMoveToward()) {
    moveToward();
  } else {
    wait();
  }
}

// void AISystem::randomMotion(entityx::Entity e) {
//   int d_x = world->rnd->getInt(-1, 1);
//   int d_y = world->rnd->getInt(-1, 1);
//   world->events.emit<Movement>(e, d_x, d_y);
// }
//
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
