#pragma once
#include <X11/extensions/randr.h>
#include <glm/glm.h>

struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;
  TransformComponent(glm::vec2 positon, glm::vec2 scale, double rotation) {
    this->postion = position;
    this->scale = scale;
    this->rotation = rotation;
  }
}
