#ifndef _WORLD_HPP
#define _WORLD_HPP

struct Position;
struct Health;
struct Render;
struct Display;
struct HealthSystem;
struct DisplaySystem;
struct MovementSystem;

struct World : std::enable_shared_from_this<World> {
  entityx::EntityX ECS;
  entityx::Entity player;
  int turnCount = 0;
  entityx::EventManager &events = ECS.events;

  World() {}

  void init(std::shared_ptr<Display> d) {
    setupPlayer();
    startSystems(d);
  }

  void setupPlayer() {
    player = ECS.entities.create();
    player.assign<Position>(20, 10);
    player.assign<Health>();
    player.assign<Render>('@');
  }

  void startSystems(std::shared_ptr<Display> d) {
    ECS.systems.add<HealthSystem>();
    ECS.systems.add<DisplaySystem>(d, shared_from_this());
    ECS.systems.add<MovementSystem>();
    ECS.systems.configure();
  }

  void updateDisplayOnly() { ECS.systems.update<DisplaySystem>(1); }

  void update() {
    ECS.systems.update_all(1);
    turnCount++;
  }
};

#endif
