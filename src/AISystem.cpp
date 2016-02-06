#include "AISystem.hpp"

void AISystem::update(entityx::EntityManager &es, entityx::EventManager &events,
                      entityx::TimeDelta dt) {
  es.each<NPC>(
      [this](entityx::Entity entity, NPC &npc) { randomMotion(entity); });
}

void AISystem::randomMotion(entityx::Entity e) {
  int d_x = world->rnd->getInt(-1, 1);
  int d_y = world->rnd->getInt(-1, 1);
  world->events.emit<Movement>(e, d_x, d_y);
}
