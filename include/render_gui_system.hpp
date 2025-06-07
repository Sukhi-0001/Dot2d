#pragma once
#include "SDL_render.h"
#include "ecs.hpp"
#include "imgui_impl_sdlrenderer2.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
class Render_gui_system : public System {
public:
  Render_gui_system() = default;
  void update(SDL_Renderer *renderer) { // draw all imgui objects
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    // to show something
    if (ImGui::Begin("spwan Enimies")) {
      ImGui::Text("here we spwan Enimies");
    }
    ImGui::End();
    //
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
  }
};
