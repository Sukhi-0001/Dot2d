#pragma once

#include "box_collider_component.hpp"
#include "ecs.hpp"
#include "events/collision_event.hpp"
#include "events/event_bus.hpp"
#include "health_component.hpp"
#include "projectile_component.hpp"
#include "spdlog/spdlog.h"
class Damage_system : public System {
public:
  Damage_system() { require_comopent<Box_collider_component>(); }
  void subscribe_to_events(std::unique_ptr<Event_bus> &event_bus) {
    event_bus->subscribe_to_event<Collision_event>(
        this, &Damage_system::on_collision);
  }
  void on_collision(Collision_event &event) {
    spdlog::info("damage system recived collison event ");
    Entity &a = event.a, &b = event.b;
    if (a.belong_to_group("projectiles") && b.has_tag("player")) {
      on_projectile_hits_player(a, b);
    }
    if (b.belong_to_group("projectiles") && a.has_tag("player")) {
      on_projectile_hits_player(b, a);
    }

    if (a.belong_to_group("projectiles") && b.belong_to_group("enemies")) {
      on_projectile_hits_enimies(a, b);
    }

    if (b.belong_to_group("projectiles") && a.belong_to_group("enimies")) {
      on_projectile_hits_enimies(b, a);
    }
  }

  void on_projectile_hits_player(Entity &projectile, Entity &player) {
    auto projectile_component =
        projectile.get_component<Projectile_component>();
    if (!projectile_component.is_friendly) {
      auto &health = player.get_component<Health_component>();
      health.health_percentage -= projectile_component.hit_percentage_damage;
      spdlog::info("player health {0}", health.health_percentage);
      if (health.health_percentage <= 0)
        player.kill();
      projectile.kill();
    }
  }

  void on_projectile_hits_enimies(Entity &projectile, Entity &enemy) {
    auto projectile_component =
        projectile.get_component<Projectile_component>();
    if (projectile_component.is_friendly) {
      spdlog::critical("here");
      auto &health = enemy.get_component<Health_component>();
      health.health_percentage -= projectile_component.hit_percentage_damage;
      // spdlog::critical("health of enemy {0}", health.health_percentage);
      if (health.health_percentage <= 0)
        enemy.kill();
      projectile.kill();
    }
  }

  void update() {}
};
