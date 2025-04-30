#pragma once
#include "ecs.h"
#include <glm/glm.hpp>

struct Sprite_component : public Base_component {
  int width;
  int height;
  Sprite_component(int width = 0, int height = 0) {
    this->width = width;
    this->height = height;
  }
};
