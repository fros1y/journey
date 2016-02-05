#ifndef _DISPLAYSYSTEM_HPP
#define _DISPLAYSYSTEM_HPP

struct World;

struct DisplaySystem : entityx::System<DisplaySystem>,
                       public entityx::Receiver<MovementSystem> {
  std::shared_ptr<Display> display;
  std::shared_ptr<World> world;

  DisplaySystem(std::shared_ptr<Display> display, std::shared_ptr<World> world)
      : display(display), world(world) {}

  void configure(entityx::EventManager &event_manager) override {
    event_manager.subscribe<Message>(*this);
  }

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override {
    display->clear();

    display->statusBar(world);

    es.each<Position, Render>(
        [this](entityx::Entity entity, Position &position, Render &render) {
          display->drawEntity(position.x, position.y, render.glyph);
        });
    display->render();
  }

  void receive(const Message &mesg) { printf("%s\n", mesg.text.c_str()); }
};

#endif
