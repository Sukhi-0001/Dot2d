#pragma once
#include "ecs.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>

struct Transform_component : public Base_component {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;
  Transform_component(glm::vec2 position = glm::vec2(0, 0),
                      glm::vec2 scale = glm::vec2(0, 0), double rotation = 0) {
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
  }
};
