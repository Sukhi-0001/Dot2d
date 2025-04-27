#pragma once
#include <glm/glm.hpp>

struct Rigid_body_component {
  glm::vec2 velocity;
  Rigid_body_component(glm::vec2 velocity = glm::vec2(0.0, 0.0)) {
    this->velocity = velocity;
  }
};
