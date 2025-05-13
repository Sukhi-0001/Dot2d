#pragma once

#include "box_collider_component.hpp"
#include "ecs.hpp"
#include "events/collision_event.hpp"
#include "events/event_bus.hpp"
#include "spdlog/spdlog.h"
class Damage_system : public System {
public:
  Damage_system() { require_comopent<Box_collider_component>(); }
  void subscribe_to_events(std::unique_ptr<Event_bus> &event_bus) {
    event_bus->subscribe_to_event<Collision_event>(
        this, &Damage_system::on_collision);
  }
  void on_collision(Collision_event &event) {
    spdlog::info("damage system recived collison event");
    event.a.kill();
    event.b.kill();
  }

  void update() {}
};
