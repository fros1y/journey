#include "Map.hpp"
#include "utils.h"
#include "MapGen.hpp"

void Map::generateLevel() {
  auto mapgen = MapGen(world, width, height);
  mapgen.generate();
  mapgen.forAll([this](const Element e, const int x, const int y){
    switch(e) {
      case Element::Rock:
        makeBlock(x, y);
        break;
      case Element::Floor:
        makeFloor(x, y);
        break;
      case Element::Door:
        makeDoor(x, y);
        break;
      default:
        break;
    }
  });
}

void Map::makeLightSource(int x, int y, float brightness, TCODColor color) {
  auto light = world->entities.create();
  light.assign<Position>(x, y);
  light.assign<LightSource>(brightness, color);
}

void Map::makeDoor(int x, int y) {
  auto floor = world->entities.create();
  floor.assign<Position>(x, y);
  floor.assign<Render>('+');
}

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

void Map::computeMovesTo(int d_x, int d_y) { d_map->calculate(d_x, d_y); }

void Map::nextStepFrom(int x, int y, int &n_x, int &n_y) {
  d_map->nextStepFrom(x, y, n_x, n_y);
}

bool Map::canReachFrom(int x, int y) { return d_map->canReachFrom(x, y); }

void Map::calculateMaps() {
  // We only need to recalculate when the turn advances.
  if (calculatedForTurn == world->turnCount)
    return;

  calculatedForTurn = world->turnCount;
  tcod_map->clear();
  d_map->clear();
  l_map->clear();

  world->entities.each<Position>([this](entityx::Entity entity,
                                        Position &position) {
    entityx::ComponentHandle<Obstruction> ob = entity.component<Obstruction>();
    bool walkable = true;
    bool transparent = true;

    if (ob) {
      walkable = !(ob->obstructs);
      transparent = !(ob->blocksView);
    }
    tcod_map->setProperties(position.x, position.y, transparent, walkable);
    d_map->setProperties(position.x, position.y, walkable);
  });

  calculateLighting();
}

void Map::calculateLighting() const {
  const float MIN_INTENSITY = 0.05;
  world->entities.each<LightSource, Position>(
      [this, MIN_INTENSITY](entityx::Entity entity, LightSource &l, Position &position) {
        world->currLevel->computeFoVFrom(position.x, position.y, 0);
        auto delta = int(sqrt(l.brightness / MIN_INTENSITY));
        for (auto i = clamp(position.x - delta, 0, width); i < clamp(position.x + delta, 0, width); i++) {
          for (auto j = clamp(position.y - delta, 0, width); j < clamp(position.y + delta, 0, width); j++) {
            if (world->currLevel->isInFoV(i, j)) {
              double distanceSqr = clamp(pow(position.x - i, 2.0) + pow(position.y - j, 2), 1.0, 1e10);
              float intensity = l.brightness * (1 / distanceSqr);
              if (intensity > MIN_INTENSITY)
                l_map->addColor(i, j, l.color, intensity);
            }
          }
        }
      });
}

void Map::computeFoVFrom(int x, int y, int range) {
  tcod_map->computeFov(x, y, range);
}

bool Map::isInFoV(int x, int y) { return tcod_map->isInFov(x, y); }

TCODColor Map::getLightAt(const int x, const int y) {
  if(world->WIZARD) {
    return TCODColor::white;
  }

  return l_map->getColor(x, y);
}
