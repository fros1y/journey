#include "Map.hpp"

void Map::makeBlock(int x, int y) {
  auto block = world->entities.create();
  block.assign<Position>(x, y);
  block.assign<Obstruction>();
  block.assign<Render>('#');
}

void Map::makeFloor(int x, int y) {
  auto floor = world->entities.create();
  floor.assign<Position>(x, y);
  floor.assign<Render>('.');
}

char Map::get(int x, int y) { return tiles[width * y + x]; }
void Map::set(int x, int y, char v) { tiles[width * y + x] = v; }

void Map::floodFill(int x, int y, char v) {
  if (get(x, y) == '#') {
    return;
  }
  if (y > 0)
    floodFill(x, y - 1, v);
  if (x > 0)
    floodFill(x - 1, y, v);
  if (x > 0 && y > 0)
    floodFill(x - 1, y - 1, v);
  if (x < width - 1)
    floodFill(x + 1, y, v);
  if (y < height - 1)
    floodFill(x, y + 1, v);
  if (x < width - 1 && y < height - 1)
    floodFill(x + 1, y + 1, v);
  if (x > 0 && y < height - 1)
    floodFill(x - 1, y + 1, v);
  if (x < width - 1 && y > 0)
    floodFill(x + 1, y - 1, v);
}

void Map::addMonsters() {
  for (auto i = 0; i < 5; ++i) {
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

void Map::toEntities() {
  for (auto i = 0; i < width; ++i) {
    for (auto j = 0; j < height; ++j) {
      if (get(i, j) == '#') {
        makeBlock(i, j);
      } else {
        makeFloor(i, j);
      }
    }
  }
}

void Map::generateCavern() {}

void Map::generateArena() {
  for (auto i = 0; i < height; ++i) {
    for (auto j = 0; j < width; ++j) {
      if (i == 0 || j == 0 || i == height - 1 || j == width - 1 ||
          (i % 7 == 0 && j % 7 == 0)) {
        set(j, i, '#');
      }
    }
  }
  floodFill(1, 1, '.');
  toEntities();
  addMonsters();
}
