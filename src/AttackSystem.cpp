#include "AttackSystem.hpp"

void AttackSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Attack>(*this);
}

void AttackSystem::receive(const Attack &attack) {
  world->events.emit<Message>("Attack!");
  world->events.emit<Damage>(attack.target, 1);
}
