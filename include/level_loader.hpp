#pragma once

#include "SDL_render.h"
#include "assets_manager.hpp"
#include "ecs.hpp"
#include <memory>
#include <sol/state.hpp>
class Level_loader {
public:
  Level_loader();
  ~Level_loader();
  void load_level(sol::state &lua, const std::unique_ptr<Registry> &registry,
                  const std::unique_ptr<Assets_manager> &assets_manager,
                  SDL_Renderer *renderer, int level);
};
