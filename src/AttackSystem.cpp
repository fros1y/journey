#include "AttackSystem.hpp"

void AttackSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Attack>(*this);
}

void AttackSystem::receive(const Attack &attack) {
  auto attacker_name = attack.source.component<Name>();
  auto attacked_name = attack.target.component<Name>();

  std::string attack_message;
  if(!attacker_name->unique) {
    attack_message += "The ";
  }
  attack_message += attacker_name->name;
  attack_message += " attacks ";
  if(!attacked_name->unique) {
    attack_message += "the ";
  }
  attack_message += attacked_name->name;
  attack_message += ".";

  world->events.emit<Message>(attack_message);
  world->events.emit<Damage>(attack.target, 1);
}
