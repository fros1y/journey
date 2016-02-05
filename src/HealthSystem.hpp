#ifndef _HEALTHSYSTEM_HPP
#define _HEALTHSYSTEM_HPP

struct HealthSystem : entityx::System<HealthSystem>,
                      public entityx::Receiver<HealthSystem> {
  std::shared_ptr<World> world;
  HealthSystem(std::shared_ptr<World> world) : world(world) {}
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {}

  void configure(entityx::EventManager &event_manager) override {
    event_manager.subscribe<Damage>(*this);
  }

  void receive(const Damage &damage) {
    entityx::ComponentHandle<Health> health = damage.target.component<Health>();
    health->currHP -= damage.amount;

    if (health->currHP <= 0) {
      world->events.emit<Message>("Dies!");
      damage.target.destroy();
    }
  }
};

#endif
