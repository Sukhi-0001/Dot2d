#pragma once

#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "assets_manager.hpp"
#include "ecs.hpp"
#include "text_label_component.hpp"
#include <cstddef>
#include <memory>
class Render_text_system : public System {
public:
  Render_text_system() { require_comopent<Text_label_component>(); }
  void update(SDL_Renderer *renderer,
              std::unique_ptr<Assets_manager> &assets_manager,
              const SDL_Rect &camera) {
    for (auto entity : get_system_entities()) {
      const auto text_label = entity.get_component<Text_label_component>();
      SDL_Surface *surface =
          TTF_RenderText_Blended(assets_manager->get_font(text_label.asset_id),
                                 text_label.text.c_str(), text_label.color);
      SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
      int width = 0, height = 0;
      SDL_QueryTexture(texture, NULL, NULL, &width, &height);
      SDL_FreeSurface(surface);
      SDL_Rect des_rect = {
          (int)(text_label.postion.x - (text_label.is_fixed ? 0 : camera.x)),
          (int)(text_label.postion.y - (text_label.is_fixed ? 0 : camera.y)),
          width, height};
      SDL_RenderCopy(renderer, texture, NULL, &des_rect);
      SDL_DestroyTexture(texture);
    }
  }
};
