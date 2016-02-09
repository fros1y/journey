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
  world->currLevel->calculateMaps();
  world->currLevel->computeFoVFrom(position->x, position->y, 0);
  es.each<Position, Render>(
      [this](entityx::Entity entity, Position &position, Render &render) {
        if (world->currLevel->isInFoV(position.x, position.y) &&
            world->currLevel->l_map->illuminated(position.x, position.y)) {
          TCODColor renderColor = render.color * world->currLevel->l_map->getColor(position.x, position.y);
          if (!entity.has_component<AI>()) render.known = true;
          display->drawEntity(position.x, position.y, render.glyph, renderColor);
        } else if (render.known)
          display->drawEntity(position.x, position.y, render.glyph, TCODColor::darkGrey);
      });

  display->drawEntity(position->x, position->y, '@', TCODColor::white);
  display->render();
}

void DisplaySystem::receive(const Message &mesg) {
  printf("%s\n", mesg.text.c_str());
}
