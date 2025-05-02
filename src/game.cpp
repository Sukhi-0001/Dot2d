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
  assets_manager->add_texture(renderer, "tank-img", "../assets/tank.png");
  Entity tank = registry->create_entity();
  tank.add_component<Rigid_body_component>(glm::vec2(10, 0));
  tank.add_component<Transform_component>(glm::vec2(10, 10), glm::vec2(2, 1),
                                          0.0);
  tank.add_component<Sprite_component>("tank-img", 300, 300);
}

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
  spdlog::info("delta time {0}", deltaTime);
  // Update the registry to process the entities that are waiting to be
  // created/deleted
  registry->update();

  // Invoke all the systems that need to update
  registry->get_system<Movement_system>().update(deltaTime);
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
  registry->get_system<Render_system>().update(renderer, assets_manager);
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
