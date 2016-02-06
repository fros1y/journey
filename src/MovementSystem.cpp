#include "MovementSystem.hpp"

bool obstructsTest(entityx::Entity e) {
  entityx::ComponentHandle<Obstruction> ob = e.component<Obstruction>();
  if (ob && ob->obstructs) {
    return true;
  } else {
    return false;
  }
}

bool AttackableTest(entityx::Entity e) {
  entityx::ComponentHandle<Attackable> attackable = e.component<Attackable>();
  if (attackable) {
    return true;
  } else {
    return false;
  }
}

int attack_filter(std::vector<entityx::Entity> &results,
                  std::vector<entityx::Entity> input) {
  std::copy_if(input.begin(), input.end(), std::back_inserter(results),
               [](entityx::Entity e) { return AttackableTest(e); });
  return results.size();
}

void MovementSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Movement>(*this);
}

void MovementSystem::receive(const Movement &move) {
  auto mover = move.target;
  if (move.d_x == 0 && move.d_y == 0)
    return;

  entityx::ComponentHandle<Position> position = mover.component<Position>();
  if (!position) {
    return;
  }

  auto destination_x = position->x + move.d_x;
  auto destination_y = position->y + move.d_y;

  std::vector<entityx::Entity> entitiesAtDest;
  std::vector<entityx::Entity> NPCsAtDest;

  bool obstruction = false;
  if (world->entitiesAt(entitiesAtDest, destination_x, destination_y)) {

    if (attack_filter(NPCsAtDest, entitiesAtDest) > 0) {
      if (!mover.component<NPC>() || !NPCsAtDest[0].component<NPC>())
        // monsters don't kill monsters
        world->events.emit<Attack>(mover, NPCsAtDest[0]);
      obstruction = true;
    } else if (std::any_of(
                   entitiesAtDest.cbegin(), entitiesAtDest.cend(),
                   [](entityx::Entity e) { return obstructsTest(e); })) {
      obstruction = true;
      world->events.emit<Message>("Bump!");
    }
  }

  if (!obstruction) {
    position->x = destination_x;
    position->y = destination_y;
  }
}
