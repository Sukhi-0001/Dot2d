#pragma once
#include "SDL_timer.h"
#include "animation_component.h"
#include "ecs.h"
#include "sprite_component.h"

class Animation_system : public System {

public:
  Animation_system() {
    require_comopent<Sprite_component>();
    require_comopent<Animation_component>();
  }
  ~Animation_system() {}

  void update() {
    for (auto entity : get_system_entities()) {
      auto &animation = entity.get_component<Animation_component>();
      auto &sprite = entity.get_component<Sprite_component>();
      animation.current_frame = ((SDL_GetTicks() - animation.start_time) *
                                 animation.frame_speed_rate / 1000) %
                                animation.num_frames;
      sprite.src_rect.x = animation.current_frame * sprite.width;
    }
  }
};
