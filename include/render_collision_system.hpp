#pragma once

#include "SDL_rect.h"
#include "SDL_render.h"
#include "box_collider_component.hpp"
#include "ecs.hpp"
#include "transform_component.hpp"
class Render_collision_system : public System {
public:
  Render_collision_system() {
    require_comopent<Box_collider_component>();
    require_comopent<Transform_component>();
  }
  ~Render_collision_system() {}
  void update(SDL_Renderer *renderer, SDL_Rect &camera) {
    for (auto entity : get_system_entities()) {
      const auto transform = entity.get_component<Transform_component>();
      const auto collider = entity.get_component<Box_collider_component>();
      SDL_Rect collider_rect = {
          (int)(transform.position.x + collider.offset.x - camera.x),
          (int)(transform.position.y + collider.offset.y - camera.y),
          (int)(collider.width * transform.scale.x),
          (int)(collider.height * transform.scale.y),
      };
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderDrawRect(renderer, &collider_rect);
    }
  }
};
