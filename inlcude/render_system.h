
#pragma once
#include "SDL_rect.h"
#include "SDL_render.h"
#include "spdlog/spdlog.h"
#include "sprite_component.h"
#include "transform_component.h"
#include <ecs.h>

class Render_system : public System {
public:
  Render_system() {
    require_comopent<Transform_component>();
    require_comopent<Sprite_component>();
  }

  void update(SDL_Renderer *renderer) {
    for (auto entity : get_system_entities()) {
      spdlog::info("I AM IN RENDER SYSTEM LOOP");
      const auto transform = entity.get_component<Transform_component>();
      const auto sprite = entity.get_component<Sprite_component>();
      SDL_Rect obj_rect = {(int)transform.position.x, (int)transform.position.y,
                           sprite.width, sprite.height};
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
      SDL_RenderFillRect(renderer, &obj_rect);
    }
  }
};
