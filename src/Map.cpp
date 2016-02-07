#include "Map.hpp"

void Map::makeBlock(int x, int y) {
  auto block = world->entities.create();
  block.assign<Position>(x, y);
  block.assign<Obstruction>();
  block.assign<Render>('#');
  block.assign<MapElement>(1);
  set(x, y, block);
}

void Map::makeFloor(int x, int y) {
  auto floor = world->entities.create();
  floor.assign<Position>(x, y);
  floor.assign<Render>('.');
  floor.assign<MapElement>(0);
  set(x, y, floor);
}

bool Map::populated(int x, int y) {
  auto e = get(x, y);
  return e == true;
}

void Map::computeMovesTo(int d_x, int d_y) { d_map->calculate(d_x, d_y); }

void Map::nextStepFrom(int x, int y, int &n_x, int &n_y) {
  d_map->nextStepFrom(x, y, n_x, n_y);
}

bool Map::canReachFrom(int x, int y) { return d_map->canReachFrom(x, y); }

void Map::calculateMaps() {
  if (tcod_map)
    delete tcod_map;
  if (d_map)
    d_map = nullptr;

  tcod_map = new TCODMap(width, height);
  d_map = std::make_shared<DjikstraMap>(width, height);
  world->entities.each<Position>(
      [this](entityx::Entity entity, Position &position) {
        bool walkable = !entity.has_component<Obstruction>();
        bool transparent = walkable;
        tcod_map->setProperties(position.x, position.y, walkable, transparent);
        d_map->setProperties(position.x, position.y, walkable);
      });
}

bool Map::obstructs(int x, int y) {
  auto e = get(x, y);
  if (!e)
    return false;
  entityx::ComponentHandle<Obstruction> ob = e.component<Obstruction>();
  if (ob && ob->obstructs) {
    return true;
  } else {
    return false;
  }
}

entityx::Entity Map::get(int x, int y) { return tiles[width * y + x]; }
void Map::set(int x, int y, entityx::Entity e) { tiles[width * y + x] = e; }

void Map::floodFill(int x, int y, std::function<void(int, int)> &builder) {
  if (populated(x, y)) {
    return;
  } else {
    builder(x, y);
  }
  if (y > 0)
    floodFill(x, y - 1, builder);
  if (x > 0)
    floodFill(x - 1, y, builder);
  if (x > 0 && y > 0)
    floodFill(x - 1, y - 1, builder);
  if (x < width - 1)
    floodFill(x + 1, y, builder);
  if (y < height - 1)
    floodFill(x, y + 1, builder);
  if (x < width - 1 && y < height - 1)
    floodFill(x + 1, y + 1, builder);
  if (x > 0 && y < height - 1)
    floodFill(x - 1, y + 1, builder);
  if (x < width - 1 && y > 0)
    floodFill(x + 1, y - 1, builder);
}

void Map::addMonsters() {
  for (auto i = 0; i < 50; ++i) {
    int m_x, m_y;
    m_x = world->rnd->getInt(0, width);
    m_y = world->rnd->getInt(0, height);

    auto enemy = world->entities.create();
    enemy.assign<Position>(m_x, m_y);
    enemy.assign<Obstruction>(true, false);
    enemy.assign<Render>('r');
    enemy.assign<NPC>("rat");
    enemy.assign<Health>(5);
    enemy.assign<Attackable>();
  }
}

void Map::generateCavern() {}

void Map::generateArena() {
  for (auto i = 0; i < height; ++i) {
    for (auto j = 0; j < width; ++j) {
      if (i == 0 || j == 0 || i == height - 1 || j == width - 1 ||
          (i % 7 == 0 && j % 7 == 0)) {
        makeBlock(j, i);
      }
    }
  }

  std::function<void(int, int)> f = [=](int x, int y) {
    makeFloor(x, y);
    return;
  };
  floodFill(1, 1, f);
  addMonsters();
}

void Map::computeFoVFrom(int x, int y, int range) {
  tcod_map->computeFov(x, y, range);
}

bool Map::isInFoV(int x, int y) { return tcod_map->isInFov(x, y); }
