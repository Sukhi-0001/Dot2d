#pragma once
#include "ecs.h"
#include <glm/glm.hpp>

struct TransformComponent : public Base_component {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;
  TransformComponent(glm::vec2 position, glm::vec2 scale, double rotation) {
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
  }
};
