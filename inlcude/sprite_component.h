#pragma once
#include "ecs.h"
#include <glm/glm.hpp>
#include <string>

struct Sprite_component : public Base_component {
  std::string asset_id;
  int width;
  int height;
  Sprite_component(std::string asset_id = "", int width = 0, int height = 0) {
    this->asset_id = asset_id;
    this->width = width;
    this->height = height;
  }
};
