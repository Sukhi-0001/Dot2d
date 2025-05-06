#pragma once

#include "SDL_timer.h"
struct Animation_component {
  int num_frames;
  int current_frame;
  int frame_speed_rate;
  bool is_loop;
  int start_time;
  Animation_component(int num_frames = 1, int frame_speed_rate = 1,
                      bool is_loop = true) {
    this->num_frames = num_frames;
    this->current_frame = 1;
    this->frame_speed_rate = frame_speed_rate;
    this->is_loop = is_loop;
    this->start_time = SDL_GetTicks();
  }
};
