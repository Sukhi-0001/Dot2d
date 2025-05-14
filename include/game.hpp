#ifndef GAME_H
#define GAME_H
#include "SDL_rect.h"
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
  SDL_Rect camera;
  std::unique_ptr<Registry> registry;
  std::unique_ptr<Assets_manager> assets_manager;
  std::unique_ptr<Event_bus> event_bus;

public:
  static int window_width;
  static int window_height;
  static int map_width;
  static int map_height;

  Game() {
    is_running = false;
    is_debug = false;
    registry = std::make_unique<Registry>();
    assets_manager = std::make_unique<Assets_manager>();
    event_bus = std::make_unique<Event_bus>();
    window_width = 800;
    window_height = 600;
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
