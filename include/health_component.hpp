#pragma once

#include "ecs.hpp"
struct Health_component : public Base_component {
  int health_percentage;
  Health_component(int health_percentage = 100) {
    this->health_percentage = health_percentage;
  }
};
