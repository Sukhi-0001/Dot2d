#pragma once
#include "SDL_rect.h"
#include "ecs.hpp"
#include <glm/glm.hpp>
#include <string>

struct Sprite_component : public Base_component {
  std::string asset_id;
  int width;
  int height;
  SDL_Rect src_rect;
  int z_index;
  bool is_fixed;
  Sprite_component(std::string asset_id = "", int width = 0, int height = 0,
                   int z_index = 0, bool is_fixed = false, int src_rect_x = 0,
                   int src_rect_y = 0) {
    this->asset_id = asset_id;
    this->width = width;
    this->height = height;
    this->z_index = z_index;
    this->is_fixed = is_fixed;
    src_rect = {src_rect_x, src_rect_y, width, height};
  }
};
