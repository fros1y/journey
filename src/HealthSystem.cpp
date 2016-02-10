#include "HealthSystem.hpp"

void HealthSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Damage>(*this);
}

void HealthSystem::receive(const Damage &damage) {
  entityx::ComponentHandle<Health> health = damage.target.component<Health>();
  health->currHP -= damage.amount;

  if (health->currHP <= 0) {

    auto attacked_name = damage.target.component<Name>();
    std::string attack_message;
    if(!attacked_name->unique) {
      attack_message += "The ";
    }
    attack_message += attacked_name->name;
    attack_message += " dies!";

    world->events.emit<Message>(attack_message);
    damage.target.destroy();
  }
}
