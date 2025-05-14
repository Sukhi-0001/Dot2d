#pragma once

#include "camera_follow_component.hpp"
#include "ecs.hpp"
#include "transform_component.hpp"
#include <SDL2/SDL.h>
#include <game.hpp>
class Camera_movement_system : public System {
public:
  Camera_movement_system() {
    require_comopent<Camera_follow_component>();
    require_comopent<Transform_component>();
  }

  void update(SDL_Rect &camera) {
    for (auto entity : get_system_entities()) {
      auto transform = entity.get_component<Transform_component>();
      if (transform.position.x + (camera.w / 2) < Game::map_width)
        camera.x = transform.position.x - (Game::window_width / 2);
      if (transform.position.y + (camera.h / 2) < Game::map_height)
        camera.y = transform.position.y - (Game::window_height / 2);

      camera.x = camera.x < 0 ? 0 : camera.x;
      camera.y = camera.y < 0 ? 0 : camera.y;
      camera.x = camera.x > camera.w ? camera.w : camera.x;
      camera.y = camera.x > camera.h ? camera.h : camera.y;
    }
  }
};
