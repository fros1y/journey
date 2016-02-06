#include "AISystem.hpp"

void AISystem::update(entityx::EntityManager &es, entityx::EventManager &events,
                      entityx::TimeDelta dt) {
  es.each<NPC>(
      [this](entityx::Entity entity, NPC &npc) { zombieMotion(entity); });
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
