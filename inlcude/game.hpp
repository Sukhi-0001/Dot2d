#ifndef GAME_H
#define GAME_H
#include "assets_manager.hpp"
#include <SDL.h>
#include <ecs.hpp>
#include <events/event_bus.hpp>
#include <memory>

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
private:
  bool is_running;
  bool is_debug;
  int milliseconds_previous_frame = 0;
  SDL_Window *window;
  SDL_Renderer *renderer;
  std::unique_ptr<Registry> registry;
  std::unique_ptr<Assets_manager> assets_manager;
  std::unique_ptr<Event_bus> event_bus;

public:
  Game() {
    is_running = false;
    is_debug = false;
    registry = std::make_unique<Registry>();
    assets_manager = std::make_unique<Assets_manager>();
  }
  void init();
  void load_level(int level);
  void setup();
  void process_input();
  void update();
  void render();
  void run();
  void destroy();
};

#endif // !GAME
