#pragma once
#include "box_collider_component.h"
#include "ecs.h"
#include "spdlog/spdlog.h"
#include "transform_component.h"

class Collision_system : public System {
public:
  Collision_system() {
    require_comopent<Box_collider_component>();
    require_comopent<Transform_component>();
  }
  ~Collision_system() {}
  void update() {
    auto entity = get_system_entities();
    for (auto i = entity.begin(); i != entity.end(); i++) {
      Entity a = *i;
      auto a_transform = a.get_component<Transform_component>();
      auto a_collider = a.get_component<Box_collider_component>();
      for (auto j = entity.begin(); j != entity.end(); j++) {
        Entity b = *j;
        if (a == b)
          continue;
        auto b_transform = b.get_component<Transform_component>();
        auto b_collider = b.get_component<Box_collider_component>();
        // check for collision between a and b
        bool is_collision_happen = check_AABB_collidon(
            a_transform.position.x + a_collider.offset.x,
            a_transform.position.y + a_collider.offset.y, a_collider.width,
            a_collider.height, b_transform.position.x + b_collider.offset.x,
            b_transform.position.y + b_collider.offset.y, b_collider.width,
            b_collider.height);
        if (is_collision_happen) {
          spdlog::info("collision detected of entity {0} and entity {1}",
                       a.get_id(), b.get_id());
        }
      }
    }
  }

private:
  bool check_AABB_collidon(double ax, double ay, double aw, double ah,
                           double bx, double by, double bw, double bh) {
    return (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by);
  }
};
