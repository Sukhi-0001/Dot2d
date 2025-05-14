#include "SDL_video.h"
#include "glm/fwd.hpp"
#include <Camera_movement_system.hpp>
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
#include <fstream>
#include <game.hpp>
#include <glm/glm.hpp>
#include <keyboard_control_component.hpp>
#include <keyboard_control_system.hpp>
#include <movement_system.hpp>
#include <render_collision_system.hpp>
#include <render_system.hpp>
#include <rigid_body_component.hpp>
#include <spdlog/spdlog.h>
#include <sprite_component.hpp>
#include <transform_component.hpp>
int Game::map_height;
int Game::map_width;
int Game::window_height;
int Game::window_width;
void Game::init() {

  int error = SDL_Init(SDL_INIT_EVERYTHING);
  if (error != 0) {
    spdlog::error("Failed to create SDL window {0}", SDL_GetError());
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

  is_running = true;
  camera.x = 0;
  camera.y = 0;
  camera.w = 800;
  camera.h = 600;
}

void Game::load_level(int level) {

  // add all the systems
  registry->add_system<Movement_system>();
  registry->add_system<Render_system>();
  registry->add_system<Animation_system>();
  registry->add_system<Collision_system>();
  registry->add_system<Render_collision_system>();
  registry->add_system<Damage_system>();
  registry->add_system<Keyboard_control_system>();
  registry->add_system<Camera_movement_system>();
  // adding assets to manger
  assets_manager->add_texture(renderer, "tank-img",
                              "../assets/images/tank-panther-up.png");
  assets_manager->add_texture(renderer, "tilemap-img",
                              "../assets/tilemaps/jungle.png");
  assets_manager->add_texture(renderer, "chopper-img",
                              "../assets/images/chopper-spritesheet.png");
  // todo load tilemap

  int tile_size = 32;
  double tile_scale = 2.0;
  int map_cols = 25;
  int map_rows = 20;
  std::fstream map_file;
  map_file.open("../assets/tilemaps/jungle.map");
  for (int y = 0; y < map_cols; y++) {
    for (int x = 0; x < map_rows; x++) {
      char ch;
      map_file.get(ch);
      int src_rect_y = std::atoi(&ch) * tile_size;
      map_file.get(ch);
      int src_rect_x = std::atoi(&ch) * tile_size;
      map_file.ignore();
      Entity tile = registry->create_entity();
      tile.add_component<Transform_component>(
          glm::vec2(x * (tile_scale * tile_size), y * (tile_scale * tile_size)),
          glm::vec2(tile_scale, tile_scale), 0);
      tile.add_component<Sprite_component>("tilemap-img", tile_size, tile_size,
                                           0, src_rect_x, src_rect_y);
    }
  }
  map_file.close();
  map_width = map_cols * tile_size * tile_scale;
  map_height = map_cols * tile_size * tile_scale;
  Entity tank = registry->create_entity();
  tank.add_component<Rigid_body_component>(glm::vec2(10, 0));
  tank.add_component<Transform_component>(glm::vec2(10, 10), glm::vec2(2, 1),
                                          0.0);
  tank.add_component<Sprite_component>("tank-img", 32, 32, 1);
  tank.add_component<Box_collider_component>(32, 32);

  Entity chopper = registry->create_entity();
  chopper.add_component<Sprite_component>("chopper-img", 32, 32, 1);
  chopper.add_component<Animation_component>(2, 24, true);
  chopper.add_component<Transform_component>(glm::vec2(100, 0), glm::vec2(2, 1),
                                             0.0);
  chopper.add_component<Box_collider_component>(32, 32);
  chopper.add_component<Rigid_body_component>(glm::vec2(20, 0));
  chopper.add_component<Keyboard_control_component>(
      glm::vec2(0, -20), glm::vec2(20, 0), glm::vec2(0, 20), glm::vec2(-20, 0));
  chopper.add_component<Camera_follow_component>();
}

void Game::setup() { load_level(1); }

void Game::process_input() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
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
}
void Game::render() {
  /*
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // Invoke all the systems that need to render
  registry->get_system<Render_system>().update(renderer);

  SDL_RenderPresent(renderer);
  */

  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);
  registry->get_system<Render_system>().update(renderer, assets_manager,
                                               camera);
  if (is_debug) {
    registry->get_system<Render_collision_system>().update(renderer);
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
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}
