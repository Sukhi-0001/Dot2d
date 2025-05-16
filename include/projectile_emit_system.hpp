#pragma once

#include "SDL_timer.h"
#include "box_collider_component.hpp"
#include "ecs.hpp"
#include "glm/fwd.hpp"
#include "projectile_emitter_component.hpp"
#include "rigid_body_component.hpp"
#include "sprite_component.hpp"
#include "transform_component.hpp"
#include <memory>
#include <projectile_component.hpp>

class Projectile_emit_system : public System {
public:
  Projectile_emit_system() {
    require_comopent<Projectile_emitter_component>();
    require_comopent<Transform_component>();
  }

  void update(std::unique_ptr<Registry> &registry) {
    for (auto entity : get_system_entities()) {
      auto &projectile_emitter =
          entity.get_component<Projectile_emitter_component>();
      auto transform = entity.get_component<Transform_component>();
      if (SDL_GetTicks() - projectile_emitter.last_emission_time >
          projectile_emitter.repeat_freq) {
        glm::vec2 projectile_pos = transform.position;
        if (entity.has_component<Sprite_component>()) {
          auto sprite = entity.get_component<Sprite_component>();
          projectile_pos.x += transform.scale.x * sprite.width / 2;
          projectile_pos.y += transform.scale.y * sprite.height / 2;
        }
        // add new projectile
        Entity projectile = registry->create_entity();
        projectile.add_component<Transform_component>(projectile_pos,
                                                      glm::vec2(1, 1), 0);
        projectile.add_component<Rigid_body_component>(
            projectile_emitter.velocity);
        projectile.add_component<Sprite_component>("bullet-img", 4, 4, 10);
        projectile.add_component<Box_collider_component>(4, 4);
        projectile_emitter.last_emission_time = SDL_GetTicks();
        projectile.add_component<Projectile_comoponent>(
            projectile_emitter.is_friendly,
            projectile_emitter.hit_percent_damage,
            projectile_emitter.projectile_duration);
      }
    }
  }
};
