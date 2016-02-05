#ifndef _HEALTHSYSTEM_HPP
#define _HEALTHSYSTEM_HPP

struct HealthSystem : entityx::System<HealthSystem> {
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {
    es.each<Health>([](entityx::Entity entity, Health &health) {});
  }
};

#endif
