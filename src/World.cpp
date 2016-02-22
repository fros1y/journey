#include "World.hpp"

void World::init(std::shared_ptr<Display> d) {
  rnd = new TCODRandom(0xdeadbeef);
  generateMap();
  setupPlayer();
  startSystems(d);
}

void World::generateMap() {
  currLevel = std::make_shared<Map>(shared_from_this(), 100, 50);
  currLevel->generateLevel();
}

void World::setupPlayer() {
  player = ECS.entities.create();
  player.assign<Position>(currLevel->playerStart);
  player.assign<Health>(1000);
  player.assign<Obstruction>();
  player.assign<Render>('@');
  player.assign<Name>("Rodney", true);
  player.assign<Attackable>();
  player.assign<Speed>(1.0);
  player.assign<LightSource>(3, TCODColor::lighterYellow);
}

void World::startSystems(std::shared_ptr<Display> d) {
  ECS.systems.add<HealthSystem>(shared_from_this());
  ECS.systems.add<DisplaySystem>(d, shared_from_this());
  ECS.systems.add<MovementSystem>(shared_from_this());
  ECS.systems.add<AttackSystem>(shared_from_this());
  ECS.systems.add<AISystem>(shared_from_this());
  ECS.systems.configure();
}

void World::updateDisplayOnly() { ECS.systems.update<DisplaySystem>(1); }

void World::update() {
  ECS.systems.update_all(1);
  turnCount++;
}

int World::entitiesAt(std::vector<entityx::Entity> &results, int x, int y) {
  ECS.entities.each<Position>(
      [this, x, y, &results](entityx::Entity entity, Position &position) {
        if (position.x == x && position.y == y) {
          results.push_back(entity);
        }
      });
  return results.size();
}
