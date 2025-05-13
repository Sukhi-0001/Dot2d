#pragma once
#include "ecs.hpp"
#include <glm/glm.hpp>

struct Box_collider_component : public Base_component {
  int width;
  int height;
  glm::vec2 offset;

  Box_collider_component(int width = 0, int height = 0,
                         glm::vec2 offset = glm::vec2(0)) {
    this->width = width;
    this->height = height;
    this->offset = offset;
  }
};
