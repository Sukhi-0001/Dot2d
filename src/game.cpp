#include <SDL.h>
#include <game.h>
#include <spdlog/spdlog.h>

void Game::init() {
  int error = SDL_Init(SDL_INIT_EVERYTHING);
  if (error != 0) {
    spdlog::error("Failed to create SDL window {0}", SDL_GetError());
  }
  SDL_Window *window =
      SDL_CreateWindow("Dot2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       800, 600, SDL_WINDOW_BORDERLESS);
  if (window == NULL) {
    spdlog::error("Failed to create SDL window");
  }
}
void Game::setup() {}

void Game::process_input() {}
void Game::update() {}
void Game::render() {}
void Game::run() {
  setup();
  while (is_running) {
    process_input();
    update();
    render();
  }
  return;
}

void Game::destroy() {}
