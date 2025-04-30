#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "ecs.h"
#include "glm/fwd.hpp"
#include <SDL.h>
#include <cmath>
#include <game.h>
#include <glm/glm.hpp>
#include <movement_system.h>
#include <render_system.h>
#include <rigid_body_component.h>
#include <spdlog/spdlog.h>
#include <sprite_component.h>
#include <transform_component.h>

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
}

void Game::setup() {
  // add all the systems
  registry->add_system<Movement_system>();
  registry->add_system<Render_system>();
  Entity tank = registry->create_entity();
  // tank.add_component<Rigid_body_component>(glm::vec2(0.01, 0.02));
  tank.add_component<Transform_component>(glm::vec2(10, 10), glm::vec2(1, 1),
                                          0.0);
  tank.add_component<Sprite_component>(10, 15);
}

void Game::process_input() {
  SDL_Event sdl_event;
  while (SDL_PollEvent(&sdl_event)) {
    switch (sdl_event.type) {
    case SDL_QUIT:
      is_running = false;
      break;
    }
  }
  SDL_PollEvent(&sdl_event);
}
void Game::update() {

  registry->update();
  // update all the systems
  registry->get_system<Movement_system>().update(); // update the registry-
}
void Game::render() {
  // SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  SDL_RenderClear(renderer);
  registry->get_system<Render_system>().update(renderer);
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
