#pragma once
#include "SDL_pixels.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "assets_manager.hpp"
#include "ecs.hpp"
#include "health_component.hpp"
#include "sprite_component.hpp"
#include "transform_component.hpp"
#include <memory>
#include <string>
class Render_health_bar_system : public System {

public:
  Render_health_bar_system() {
    require_comopent<Health_component>();
    require_comopent<Transform_component>();
    require_comopent<Sprite_component>();
  }
  void update(SDL_Renderer *renderer,
              std::unique_ptr<Assets_manager> &assets_manager,
              const SDL_Rect &camera) {
    for (auto entity : get_system_entities()) {
      const auto transform = entity.get_component<Transform_component>();
      const auto sprite = entity.get_component<Sprite_component>();
      const auto health = entity.get_component<Health_component>();

      SDL_Color health_bar_component_color = {255, 255, 255};
      if (health.health_percentage >= 0 && health.health_percentage < 40) {
        // 0-40 red
        health_bar_component_color = {255, 0, 0};
      }
      if (health.health_percentage >= 40 && health.health_percentage < 80) {
        // 0-40 yelow
        health_bar_component_color = {255, 255, 0};
      }
      if (health.health_percentage >= 80 && health.health_percentage < 100) {
        // 0-40 green
        health_bar_component_color = {0, 255, 0};
      }
      int health_bar_width = 15;
      int health_bar_height = 3;
      double health_bar_pos_x =
          (transform.position.x + (sprite.width * transform.scale.x)) -
          camera.x;
      double health_bar_pos_y = transform.position.y - camera.y;
      SDL_Rect health_bar_rectangle = {
          (int)(health_bar_pos_x), (int)(health_bar_pos_y),
          (int)(health_bar_width * (health.health_percentage / 100.0)),
          (int)(health_bar_height)};
      SDL_SetRenderDrawColor(renderer, health_bar_component_color.r,
                             health_bar_component_color.g,
                             health_bar_component_color.b, 255);
      SDL_RenderFillRect(renderer, &health_bar_rectangle);
      std::string health_text = std::to_string(health.health_percentage);
      SDL_Surface *surface = TTF_RenderText_Blended(
          assets_manager->get_font("pico8-font-5"), health_text.c_str(),
          health_bar_component_color);
      SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);
      int label_width = 0;
      int label_height = 0;
      SDL_QueryTexture(texture, NULL, NULL, &label_width, &label_height);
      SDL_Rect health_bar_text_rect = {(int)(health_bar_pos_x),
                                       (int)(health_bar_pos_y) + 5, label_width,
                                       label_height};
      SDL_RenderCopy(renderer, texture, NULL, &health_bar_text_rect);
      SDL_DestroyTexture(texture);
    }
  }
};
