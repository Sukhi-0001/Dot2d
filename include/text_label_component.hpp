#pragma once

#include "SDL_pixels.h"
#include <glm/glm.hpp>
#include <string>
struct Text_label_component {
  glm::vec2 postion;
  std::string text;
  std::string asset_id;
  SDL_Color color;
  bool is_fixed;
  Text_label_component(glm::vec2 postion = glm::vec2(0), std::string text = "",
                       std::string asset_id = "",
                       const SDL_Color color = {0, 0, 0},
                       bool is_fixed = true) {
    this->postion = postion;
    this->text = text;
    this->asset_id = asset_id;
    this->color = color;
    this->is_fixed = is_fixed;
  }
};
