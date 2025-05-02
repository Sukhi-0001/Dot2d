#pragma once
#include "SDL_rect.h"
#include "SDL_render.h"
#include "assets_manager.h"
#include "spdlog/spdlog.h"
#include "sprite_component.h"
#include "transform_component.h"
#include <ecs.h>
#include <memory>

class Render_system : public System {
public:
  Render_system() {
    require_comopent<Transform_component>();
    require_comopent<Sprite_component>();
  }

  void update(SDL_Renderer *renderer,
              std::unique_ptr<Assets_manager> &assets_manager) {
    for (auto entity : get_system_entities()) {
      const auto transform = entity.get_component<Transform_component>();
      const auto sprite = entity.get_component<Sprite_component>();
      // set the source rectange of the sprite
      SDL_Rect src_rect = sprite.src_rect;
      SDL_Rect des_rect = {(int)transform.position.x, (int)transform.position.y,
                           (int)(sprite.width * transform.scale.x),
                           (int)(sprite.height * transform.scale.y)};
      SDL_RenderCopyEx(renderer, assets_manager->get_texture(sprite.asset_id),
                       &src_rect, &des_rect, transform.rotation, NULL,
                       SDL_FLIP_NONE);
      spdlog::info("Rendering image");
    }
  }
};
