#pragma once
#include "spdlog/spdlog.h"
#include "transform_component.h"
#include <ecs.h>
#include <rigid_body_component.h>

class Movement_system : public System {
public:
  Movement_system() {
    require_comopent<Transform_component>();
    require_comopent<Rigid_body_component>();
  }

  void update() {
    for (auto entity : get_system_entities()) {
      auto &transform = entity.get_component<Transform_component>();

      auto &rigid_body = entity.get_component<Rigid_body_component>();

      transform.position.x += rigid_body.velocity.x;

      transform.position.y += rigid_body.velocity.y;

      spdlog::info("entity {0} moved x by {1} and y by {2}", entity.get_id(),
                   std::to_string(transform.position.x),
                   std::to_string(transform.position.y));
    }
  }
};
