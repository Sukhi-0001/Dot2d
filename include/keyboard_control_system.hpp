#pragma once
#include "SDL_keycode.h"
#include "ecs.hpp"
#include "events/event_bus.hpp"
#include "events/key_press_event.hpp"
#include "keyboard_control_component.hpp"
#include "rigid_body_component.hpp"
#include "spdlog/spdlog.h"
#include "sprite_component.hpp"

class Keyboard_control_system : public System {
public:
  Keyboard_control_system() {
    require_comopent<Keyboard_control_component>();
    require_comopent<Sprite_component>();
    require_comopent<Rigid_body_component>();
  }

  void subscribe_to_events(std::unique_ptr<Event_bus> &event_bus) {
    event_bus->subscribe_to_event<Key_press_event>(
        this, &Keyboard_control_system::on_key_pressed);
  }

  void on_key_pressed(Key_press_event &event) {
    for (auto enitity : get_system_entities()) {
      auto keyboard_control =
          enitity.get_component<Keyboard_control_component>();
      auto &sprite = enitity.get_component<Sprite_component>();
      auto &rigid_body = enitity.get_component<Rigid_body_component>();
      switch (event.symbol) {
      case SDLK_UP:
        rigid_body.velocity = keyboard_control.up_velocity;
        sprite.src_rect.y = sprite.height * 0;
        break;
      case SDLK_RIGHT:
        rigid_body.velocity = keyboard_control.right_velocity;
        sprite.src_rect.y = sprite.height * 1;
        break;
      case SDLK_DOWN:
        rigid_body.velocity = keyboard_control.down_velocity;
        sprite.src_rect.y = sprite.height * 2;
        break;
      case SDLK_LEFT:
        rigid_body.velocity = keyboard_control.left_velocity;
        sprite.src_rect.y = sprite.height * 3;
        break;
      }
    }
  }

  void update() {}
};
