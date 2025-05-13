#pragma once
#include "ecs.hpp"
#include "events/event_bus.hpp"
#include "events/key_press_event.hpp"
#include "rigid_body_component.hpp"
#include "spdlog/spdlog.h"
#include "sprite_component.hpp"
#include <string>
class Keyboard_control_system : public System {
public:
  Keyboard_control_system() {
    require_comopent<Sprite_component>();
    require_comopent<Rigid_body_component>();
  }

  void subscribe_to_events(std::unique_ptr<Event_bus> &event_bus) {
    event_bus->subscribe_to_event<Key_press_event>(
        this, &Keyboard_control_system::on_key_pressed);
  }

  void on_key_pressed(Key_press_event &event) {
    spdlog::info("key pressed {0}", std::to_string(event.symbol));
  }

  void update() {}
};
