#pragma once
#include "SDL_timer.h"
#include "ecs.hpp"
#include <glm/glm.hpp>
struct Projectile_emitter_component : public Base_component {
  glm::vec2 velocity;
  int repeat_freq;
  int projectile_duration;
  int hit_percent_damage;
  bool is_friendly;
  int last_emission_time;
  Projectile_emitter_component(glm::vec2 velocity = glm::vec2(0),
                               int repeat_freq = 0,
                               int projectile_duration = 10000,
                               int hit_percent_damage = 10,
                               bool is_friendly = false) {
    this->velocity = velocity;
    this->repeat_freq = repeat_freq;
    this->projectile_duration = projectile_duration;
    this->hit_percent_damage = hit_percent_damage;
    this->is_friendly = is_friendly;
    this->last_emission_time = SDL_GetTicks();
  }
};
