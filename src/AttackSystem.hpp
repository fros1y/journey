#ifndef _ATTACKSYSTEM_HPP
#define _ATTACKSYSTEM_HPP

struct AttackSystem : entityx::System<AttackSystem>,
                      public entityx::Receiver<AttackSystem> {
  std::shared_ptr<World> world;

  AttackSystem(std::shared_ptr<World> world) : world(world) {}

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {}

  void configure(entityx::EventManager &event_manager) override {
    event_manager.subscribe<Attack>(*this);
  }

  void receive(const Attack &attack) {
    world->events.emit<Message>("Attack!");
    world->events.emit<Damage>(attack.target, 1);
  }
};

#endif
