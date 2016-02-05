#ifndef _WORLD_HPP
#define _WORLD_HPP

struct Position;
struct Health;
struct Render;
struct Display;
struct HealthSystem;
struct DisplaySystem;
struct MovementSystem;
struct AttackSystem;

struct World : std::enable_shared_from_this<World> {
  entityx::EntityX ECS;
  entityx::Entity player;
  int turnCount = 0;
  entityx::EventManager &events = ECS.events;
  entityx::EntityManager &entities = ECS.entities;

  World() {}

  void init(std::shared_ptr<Display> d) {
    setupPlayer();
    generateMap();
    startSystems(d);
  }

  void generateMap() {
    auto block = ECS.entities.create();
    block.assign<Position>(25, 15);
    block.assign<Obstruction>();
    block.assign<Render>('#');

    auto enemy = ECS.entities.create();
    enemy.assign<Position>(15, 5);
    enemy.assign<Obstruction>(true, false);
    enemy.assign<Render>('r');
    enemy.assign<NPC>("rat");
    enemy.assign<Health>(5);
  }

  void setupPlayer() {
    player = ECS.entities.create();
    player.assign<Position>(20, 10);
    player.assign<Health>(100);
    player.assign<Obstruction>();
    player.assign<Render>('@');
  }

  void startSystems(std::shared_ptr<Display> d) {
    ECS.systems.add<HealthSystem>(shared_from_this());
    ECS.systems.add<DisplaySystem>(d, shared_from_this());
    ECS.systems.add<MovementSystem>(shared_from_this());
    ECS.systems.add<AttackSystem>(shared_from_this());
    ECS.systems.configure();
  }

  void updateDisplayOnly() { ECS.systems.update<DisplaySystem>(1); }

  void update() {
    ECS.systems.update_all(1);
    turnCount++;
  }

  int entitiesAt(std::vector<entityx::Entity> &results, int x, int y) {
    ECS.entities.each<Position>(
        [this, x, y, &results](entityx::Entity entity, Position &position) {
          if (position.x == x && position.y == y) {
            results.push_back(entity);
          }
        });
    return results.size();
  }
};

#endif
