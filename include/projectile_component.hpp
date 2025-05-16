#pragma once
#include <SDL2/SDL.h>

struct Projectile_comoponent {
  bool is_friendly;
  int hit_percentage_damage;
  int duration;
  int start_time;
  Projectile_comoponent(bool is_friendly = false, int hit_percentage_damage = 0,
                        int duration = 0) {
    this->is_friendly = is_friendly;
    this->hit_percentage_damage = hit_percentage_damage;
    this->duration = duration;
    this->start_time = SDL_GetTicks();
  }
};
