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

  es.each<Position, Render>(
      [this](entityx::Entity entity, Position &position, Render &render) {
        display->drawEntity(position.x, position.y, render.glyph);
      });
  display->render();
}

void DisplaySystem::receive(const Message &mesg) {
  printf("%s\n", mesg.text.c_str());
}
