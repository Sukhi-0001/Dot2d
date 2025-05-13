#pragma once
#include "../ecs.hpp"
#include "event.hpp"

class Collision_event : public Event {
public:
  Entity a;
  Entity b;
  Collision_event(Entity a, Entity b) : a(a), b(b) {}
  ~Collision_event() {}
};
