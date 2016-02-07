#include "DisplaySystem.hpp"

void DisplaySystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<Message>(*this);
}

void DisplaySystem::update(entityx::EntityManager &es,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {
  display->clear();

  display->statusBar(world);

  entityx::ComponentHandle<Position> position =
      world->player.component<Position>();

  display->camera_x = position->x - display->width / 2;
  display->camera_y = position->y - display->height / 2;

  world->currLevel->tcod_map->computeFov(position->x, position->y, 3);
  es.each<Position, Render>(
      [this](entityx::Entity entity, Position &position, Render &render) {
        if (render.known)
          display->drawEntity(position.x, position.y, render.glyph);
        if (world->currLevel->tcod_map->isInFov(position.x, position.y)) {
          if (!entity.has_component<NPC>())
            render.known = true;
          display->drawEntity(position.x, position.y, render.glyph);
        }
      });

  display->drawEntity(position->x, position->y, '@');
  display->render();
}

void DisplaySystem::receive(const Message &mesg) {
  printf("%s\n", mesg.text.c_str());
}
