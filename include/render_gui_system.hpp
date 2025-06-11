#pragma once
#include "SDL_render.h"
#include "box_collider_component.hpp"
#include "ecs.hpp"
#include "health_component.hpp"
#include "imgui_impl_sdlrenderer2.h"
#include "projectile_emitter_component.hpp"
#include "rigid_body_component.hpp"
#include "sprite_component.hpp"
#include "transform_component.hpp"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <memory>
class Render_gui_system : public System {
public:
  Render_gui_system() = default;
  void
  update(SDL_Renderer *renderer,
         const std::unique_ptr<Registry> &registry) { // draw all imgui objects
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    // to show something
    if (ImGui::Begin("Spwan Enimies")) {
      static int enemy_pos_x = 0;
      ImGui::InputInt("X Position", &enemy_pos_x);
      static int enemy_pos_y = 0;
      ImGui::InputInt("Y Position", &enemy_pos_y);
      if (ImGui::Button("Create New Enemy")) {
        // create a new entity
        Entity enemy = registry->create_entity();
        enemy.group("enimies");
        enemy.add_component<Rigid_body_component>(glm::vec2(10, 0));
        enemy.add_component<Transform_component>(
            glm::vec2(enemy_pos_x, enemy_pos_y), glm::vec2(2, 1), 0.0);
        enemy.add_component<Sprite_component>("tank-img", 32, 32, 1);
        enemy.add_component<Box_collider_component>(32, 32);
        enemy.add_component<Health_component>(50);
        enemy.add_component<Projectile_emitter_component>(
            glm::vec2(150, 150), 1000, 1000, 10, false);
      }
    }
    ImGui::End();
    //
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
  }
};
