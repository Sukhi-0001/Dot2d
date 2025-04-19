#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <SDL.h>
#include <cstddef>
#include <game.h>
#include <spdlog/spdlog.h>

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

void Game::setup() {}

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
void Game::update() {}
void Game::render() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  SDL_RenderClear(renderer);
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
