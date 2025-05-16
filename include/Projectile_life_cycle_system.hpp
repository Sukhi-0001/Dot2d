#pragma once

#include "ecs.hpp"
#include "projectile_component.hpp"

class Projectile_life_cycle_system : public System {
public:
  Projectile_life_cycle_system() { require_comopent<Projectile_comoponent>(); }

  void update() {
    for (auto entity : get_system_entities()) {
      auto projectile = entity.get_component<Projectile_comoponent>();
      // kill projectile after the limit
      if (SDL_GetTicks() - projectile.start_time > projectile.duration) {
        entity.kill();
      }
    }
  }
};
