#include "HealthSystem.hpp"

void HealthSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Damage>(*this);
}

void HealthSystem::receive(const Damage &damage) {
  entityx::ComponentHandle<Health> health = damage.target.component<Health>();
  health->currHP -= damage.amount;

  if (health->currHP <= 0) {
    world->events.emit<Message>("Dies!");
    damage.target.destroy();
  }
}
