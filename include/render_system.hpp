#pragma once
#include "SDL_rect.h"
#include "SDL_render.h"
#include "assets_manager.hpp"
#include "sprite_component.hpp"
#include "transform_component.hpp"
#include <ecs.hpp>
#include <memory>
#include <vector>

class Render_system : public System {
public:
  Render_system() {
    require_comopent<Transform_component>();
    require_comopent<Sprite_component>();
  }

  void update(SDL_Renderer *renderer,
              std::unique_ptr<Assets_manager> &assets_manager,
              SDL_Rect camera) {
    struct Renderable_entity {
      Transform_component transform;
      Sprite_component sprite;
    };
    // sort all the entities
    std::vector<Renderable_entity> Renderable_entities;
    for (auto entity : get_system_entities()) {
      Renderable_entity renderable_entity;
      renderable_entity.transform = entity.get_component<Transform_component>();
      renderable_entity.sprite = entity.get_component<Sprite_component>();
      Renderable_entities.push_back(renderable_entity);
    }
    // sort the vector with z index
    std::sort(Renderable_entities.begin(), Renderable_entities.end(),
              [](const Renderable_entity &a, const Renderable_entity &b) {
                return a.sprite.z_index < b.sprite.z_index;
              });
    for (auto entity : Renderable_entities) {
      const auto transform = entity.transform;
      const auto sprite = entity.sprite;
      // set the source rectange of the sprite
      SDL_Rect src_rect = sprite.src_rect;
      SDL_Rect des_rect = {
          (int)transform.position.x - (sprite.is_fixed ? 0 : camera.x),
          (int)transform.position.y - (sprite.is_fixed ? 0 : camera.y),
          (int)(sprite.width * transform.scale.x),
          (int)(sprite.height * transform.scale.y)};
      SDL_RenderCopyEx(renderer, assets_manager->get_texture(sprite.asset_id),
                       &src_rect, &des_rect, transform.rotation, NULL,
                       SDL_FLIP_NONE);
      // spdlog::info("Rendering image");
    }
  }
};
