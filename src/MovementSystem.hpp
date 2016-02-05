#ifndef _MOVEMENTSYSTEM_HPP
#define _MOVEMENTSYSTEM_HPP

struct MovementSystem : entityx::System<MovementSystem>,
                        public entityx::Receiver<MovementSystem> {
  MovementSystem() {}

  void update(entityx::EntityManager &entities, entityx::EventManager &events,
              entityx::TimeDelta dt) {}

  void configure(entityx::EventManager &event_manager) {
    event_manager.subscribe<Movement>(*this);
  }

  void receive(const Movement &move) {
    auto target = move.target;

    entityx::ComponentHandle<Position> position = target.component<Position>();
    position->x += move.d_x;
    position->y += move.d_y;
  }
};

#endif
