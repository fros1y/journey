#ifndef _MOVEMENTSYSTEM_HPP
#define _MOVEMENTSYSTEM_HPP

bool obstructsTest(entityx::Entity e) {
  entityx::ComponentHandle<Obstruction> ob = e.component<Obstruction>();
  if (ob && ob->obstructs) {
    return true;
  } else {
    return false;
  }
}

bool NPCTest(entityx::Entity e) {
  entityx::ComponentHandle<NPC> npc = e.component<NPC>();
  if (npc) {
    return true;
  } else {
    return false;
  }
}

int npc_filter(std::vector<entityx::Entity> &results,
               std::vector<entityx::Entity> input) {
  std::copy_if(input.begin(), input.end(), std::back_inserter(results),
               [](entityx::Entity e) { return NPCTest(e); });
  return results.size();
}

struct MovementSystem : entityx::System<MovementSystem>,
                        public entityx::Receiver<MovementSystem> {
  std::shared_ptr<World> world;
  MovementSystem(std::shared_ptr<World> world) : world(world) {}

  void update(entityx::EntityManager &entities, entityx::EventManager &events,
              entityx::TimeDelta dt) {}

  void configure(entityx::EventManager &event_manager) {
    event_manager.subscribe<Movement>(*this);
  }

  void receive(const Movement &move) {
    auto mover = move.target;

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

      if (npc_filter(NPCsAtDest, entitiesAtDest) > 0) {
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
};

#endif
