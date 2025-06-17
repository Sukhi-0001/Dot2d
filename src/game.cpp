#include "SDL_ttf.h"
#include "SDL_video.h"
#include "level_loader.hpp"
#include <Camera_movement_system.hpp>
#include <Projectile_life_cycle_system.hpp>
#include <SDL.h>
#include <animation_component.hpp>
#include <animation_system.hpp>
#include <box_collider_component.hpp>
#include <camera_follow_component.hpp>
#include <cmath>
#include <collision_system.hpp>
#include <cstdlib>
#include <damage_system.hpp>
#include <ecs.hpp>
#include <events/event_bus.hpp>
#include <game.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <keyboard_control_component.hpp>
#include <keyboard_control_system.hpp>
#include <movement_system.hpp>
#include <projectile_emit_system.hpp>
#include <projectile_emitter_component.hpp>
#include <render_collision_system.hpp>
#include <render_gui_system.hpp>
#include <render_health_bar_system.hpp>
#include <render_system.hpp>
#include <render_text_system.hpp>
#include <rigid_body_component.hpp>
#include <sol/sol.hpp>
#include <sol/types.hpp>
#include <spdlog/spdlog.h>
#include <sprite_component.hpp>
#include <text_label_component.hpp>
#include <transform_component.hpp>
int Game::map_height;
int Game::map_width;
int Game::window_height;
int Game::window_width;
void test_lua() {
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  lua.script_file("../assets/scripts/launchpad.lua");
  bool is_fullscreen = lua["config"]["fullscreen"];
  sol::table config = lua["config"];
  int width = config["resolution"]["width"];
  int height = config["resolution"]["height"];
  spdlog::info("here {0} {1}", width, height);
  // exit(0);
}

void Game::init() {

  test_lua();
  int error = SDL_Init(SDL_INIT_EVERYTHING);
  if (error != 0) {
    spdlog::error("Failed to create SDL window {0}", SDL_GetError());
  }
  if (TTF_Init() != 0) {
    spdlog::error("TTF not init");
  }
  window =
      SDL_CreateWindow("Dot2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       800, 600, SDL_WINDOW_BORDERLESS);
  if (window == NULL) {
    spdlog::error("Failed to create SDL window");
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    spdlog::error("Failed to create SDL renderer");
  }
  // setup imgu
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls;

  is_running = true;
  camera.x = 0;
  camera.y = 0;
  camera.w = 800;
  camera.h = 600;
}

void Game::setup() {
  // add all the systems
  registry->add_system<Movement_system>();
  registry->add_system<Render_system>();
  registry->add_system<Animation_system>();
  registry->add_system<Collision_system>();
  registry->add_system<Render_collision_system>();
  registry->add_system<Damage_system>();
  registry->add_system<Keyboard_control_system>();
  registry->add_system<Camera_movement_system>();
  registry->add_system<Projectile_emit_system>();
  registry->add_system<Projectile_life_cycle_system>();
  registry->add_system<Render_text_system>();
  registry->add_system<Render_health_bar_system>();
  registry->add_system<Render_gui_system>();
  Level_loader loader;
  lua.open_libraries(sol::lib::base);
  loader.load_level(lua, registry, assets_manager, renderer, 1);
}

void Game::process_input() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    // imgui SDL events
    ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
    switch (sdlEvent.type) {
    case SDL_QUIT:
      is_running = false;
      break;
    case SDL_KEYDOWN:
      if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
        is_running = false;
      }
      if (sdlEvent.key.keysym.sym == SDLK_d) {
        is_debug = !is_debug;
      }
      event_bus->emit_event<Key_press_event>(sdlEvent.key.keysym.sym);
      break;
    }
  }
}
void Game::update() {

  // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
  int timeToWait =
      MILLISECONDS_PER_FRAME - (SDL_GetTicks() - milliseconds_previous_frame);
  if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) {
    SDL_Delay(timeToWait);
  }

  // The difference in ticks since the last frame, converted to seconds
  double deltaTime = (SDL_GetTicks() - milliseconds_previous_frame) / 1000.0;

  // Store the "previous" frame time
  milliseconds_previous_frame = SDL_GetTicks();
  // spdlog::info("delta time {0}", deltaTime);
  // reset all the event handerls
  event_bus->reset();
  // subsribe to all the events
  registry->get_system<Damage_system>().subscribe_to_events(event_bus);
  registry->get_system<Keyboard_control_system>().subscribe_to_events(
      event_bus);
  registry->get_system<Projectile_emit_system>().subscribe_to_events(event_bus);
  // Update the registry to process the entities that are waiting to be
  // created/deleted
  registry->update();

  // Invoke all the systems that need to update
  registry->get_system<Movement_system>().update(deltaTime);
  registry->get_system<Animation_system>().update();
  registry->get_system<Collision_system>().update(event_bus);
  registry->get_system<Damage_system>().update();
  registry->get_system<Keyboard_control_system>().update();
  registry->get_system<Camera_movement_system>().update(camera);
  registry->get_system<Projectile_emit_system>().update(registry);
  registry->get_system<Projectile_life_cycle_system>().update();
}
void Game::render() {

  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);
  registry->get_system<Render_system>().update(renderer, assets_manager,
                                               camera);
  registry->get_system<Render_text_system>().update(renderer, assets_manager,
                                                    camera);
  registry->get_system<Render_health_bar_system>().update(
      renderer, assets_manager, camera);
  if (is_debug) {
    registry->get_system<Render_collision_system>().update(renderer, camera);
    registry->get_system<Render_gui_system>().update(renderer, registry);
  }
  SDL_RenderPresent(renderer);
}
void Game::run() {
  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }
  return;
}

void Game::destroy() {
  // Cleanup
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}
